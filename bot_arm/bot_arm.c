#include "bot_arm.h"
#include <math.h>

// 机械臂配置结构体 - 存储每个机械臂的特定配置参数
typedef struct {
    node claw_node; // 抓手对应的节点
    float open_angle; // 抓手打开角度
    float close_angle; // 抓手闭合角度
    float angle_limits[node_MAX][2]; // 每个节点的角度限制 [最小值, 最大值]
    uint8_t movable_nodes[node_MAX]; // 可移动节点标记 (1=可移动, 0=不可移动)

    // 新增平滑移动配置
    uint16_t move_duration_ms;    // 移动总时间(毫秒)
    uint16_t update_interval_ms;  // 更新间隔(毫秒)
    float max_angle_step;         // 最大单步角度变化
} BotArm_Config;

// 机械臂配置数组 - 每个机械臂可以有不同配置
static BotArm_Config arm_configs[] = {
    {
        // botarm_0 配置
        node_5, // 抓手节点为 node_5
        100.0f, // 抓手打开角度
        135.0f, // 抓手闭合角度
        {
            // 每个节点的角度限制
            {0, 180}, // node_0 角度范围
            {0, 180}, // node_1 角度范围
            {0, 180}, // node_2 角度范围
            {0, 180}, // node_3 角度范围
            {0, 180}, // node_4 角度范围
            {0, 180} // node_5 角度范围
        },
        {1, 1, 1, 1, 1, 0}, // 可移动节点标记：node_5不可通过move_to移动

        // 新增平滑移动参数
        2000,    // move_duration_ms: 总移动时间2秒
        50,      // update_interval_ms: 每50ms更新一次
        5.0f     // max_angle_step: 最大单步角度变化5度
    },
};

// ============================================================================
// 内部实现函数
// ============================================================================

/**
 * @brief 机械臂初始化实现
 * @param P_BotArm 机械臂设备指针
 * @return 成功返回0，失败返回错误码
 */
static int BotArm_Init(struct BotArm *P_BotArm)
{
    if (P_BotArm == NULL) {
        return -1;
    }

    // 初始化PCA9685等底层硬件
    PCA9685_Init(&hi2c1);

    return 0; // 初始化成功
}

/**
 * @brief 设置节点角度实现
 * @param P_BotArm 机械臂设备指针
 * @param Channel 节点编号
 * @param angle 目标角度
 * @return 成功返回0，失败返回错误码
 */
static int BotArm_SetAngle(struct BotArm *P_BotArm, node Channel, float angle)
{
    // 参数验证
    if (P_BotArm == NULL) {
        return -1;
    }

    int which = P_BotArm->which;

    if (which < 0 || which >= botarm_MAX) {
        return -2; // 无效的机械臂标识
    }

    if (Channel < node_0 || Channel >= node_MAX) {
        return -3; // 无效的节点编号
    }

    // 获取该机械臂的配置
    BotArm_Config *config = &arm_configs[which];

    // 角度范围检查
    if (angle < config->angle_limits[Channel][0] ||
        angle > config->angle_limits[Channel][1]) {
        return -4; // 角度超出限制
    }

    // 调用底层PCA9685驱动设置角度
    PCA9685_SetServoAngle(Channel, angle);
    P_BotArm->LastAngl[Channel] = angle;

    return 0; // 设置成功
}

/**
 * @brief 平滑移动单个节点到目标角度
 * @param P_BotArm 机械臂设备指针
 * @param Channel 节点编号
 * @param target_angle 目标角度
 * @return 成功返回0，失败返回错误码
 */
static int BotArm_SmoothSetAngle(struct BotArm *P_BotArm, node Channel, float target_angle)
{
    if (P_BotArm == NULL) {
        return -1;
    }

    int which = P_BotArm->which;
    if (which < 0 || which >= botarm_MAX) {
        return -2;
    }

    if (Channel < node_0 || Channel >= node_MAX) {
        return -3;
    }

    // 获取该机械臂的配置
    BotArm_Config *config = &arm_configs[which];

    // 角度范围检查
    if (target_angle < config->angle_limits[Channel][0] ||
        target_angle > config->angle_limits[Channel][1]) {
        return -4;
    }

    float current_angle = P_BotArm->LastAngl[Channel];

    // 如果角度变化很小，直接设置
    if (fabsf(target_angle - current_angle) < 0.5f) {
        PCA9685_SetServoAngle(Channel, target_angle);
        P_BotArm->LastAngl[Channel] = target_angle;
        return 0;
    }

    // 计算总步数和每步角度
    float angle_diff = target_angle - current_angle;
    uint32_t total_steps = config->move_duration_ms / config->update_interval_ms;
    float angle_step = angle_diff / total_steps;

    // 限制单步最大角度变化
    if (fabsf(angle_step) > config->max_angle_step) {
        angle_step = (angle_step > 0) ? config->max_angle_step : -config->max_angle_step;
        total_steps = (uint32_t)(fabsf(angle_diff) / config->max_angle_step);
    }

    // 执行平滑移动
    for (uint32_t step = 1; step <= total_steps; step++) {
        float intermediate_angle = current_angle + angle_step * step;
        PCA9685_SetServoAngle(Channel, intermediate_angle);
        delay_ms(config->update_interval_ms);
    }

    // 确保最终位置准确
    PCA9685_SetServoAngle(Channel, target_angle);
    P_BotArm->LastAngl[Channel] = target_angle;

    return 0;
}

/**
 * @brief 多关节协调平滑移动
 * @param P_BotArm 机械臂设备指针
 * @param target_angles 目标角度数组
 * @param num_nodes 节点数量
 * @return 成功返回0，失败返回错误码
 */
static int BotArm_CoordinatedMove(struct BotArm *P_BotArm, float target_angles[], uint8_t num_nodes)
{
    if (P_BotArm == NULL || target_angles == NULL) {
        return -1;
    }

    int which = P_BotArm->which;
    BotArm_Config *config = &arm_configs[which];

    // 计算每个节点的角度差和最大角度差
    float max_angle_diff = 0;
    float angle_diffs[node_MAX];

    for (node i = 0; i < num_nodes; i++) {
        angle_diffs[i] = target_angles[i] - P_BotArm->LastAngl[i];
        if (fabsf(angle_diffs[i]) > max_angle_diff) {
            max_angle_diff = fabsf(angle_diffs[i]);
        }
    }

    // 如果所有角度变化都很小，直接设置
    if (max_angle_diff < 0.5f) {
        for (node i = 0; i < num_nodes; i++) {
            PCA9685_SetServoAngle(i, target_angles[i]);
            P_BotArm->LastAngl[i] = target_angles[i];
        }
        return 0;
    }

    // 计算总步数（基于最大角度变化）
    uint32_t total_steps = (uint32_t)(max_angle_diff / config->max_angle_step);
    if (total_steps < 1) total_steps = 1;

    // 限制最小和最大步数
    uint32_t min_steps = config->move_duration_ms / config->update_interval_ms / 2;
    uint32_t max_steps = config->move_duration_ms / config->update_interval_ms * 2;

    if (total_steps < min_steps) total_steps = min_steps;
    if (total_steps > max_steps) total_steps = max_steps;

    // 执行协调移动
    for (uint32_t step = 1; step <= total_steps; step++) {
        float t = (float)step / total_steps; // 归一化进度 [0,1]

        for (node i = 0; i < num_nodes; i++) {
            // 使用缓动函数使移动更自然（可选）
            float eased_t = t; // 可以替换为 easeInOutCubic(t) 等缓动函数
            float intermediate_angle = P_BotArm->LastAngl[i] + angle_diffs[i] * eased_t;
            PCA9685_SetServoAngle(i, intermediate_angle);
        }
        delay_ms(config->update_interval_ms);
    }

    // 确保最终位置准确
    for (node i = 0; i < num_nodes; i++) {
        PCA9685_SetServoAngle(i, target_angles[i]);
        P_BotArm->LastAngl[i] = target_angles[i];
    }

    return 0;
}

/**
 * @brief 平滑移动所有可移动节点（顺序移动，非协调）
 * @param P_BotArm 机械臂设备指针
 * @param angles 目标角度数组
 * @return 成功返回0，失败返回错误码
 */
static int BotArm_SmoothMoveTo(struct BotArm *P_BotArm, float angles[])
{
    // 参数验证
    if (P_BotArm == NULL) {
        return -1;
    }

    if (angles == NULL) {
        return -2; // 角度数组为空
    }

    int which = P_BotArm->which;

    if (which < 0 || which >= botarm_MAX) {
        return -3; // 无效的机械臂标识
    }

    // 获取该机械臂的配置
    BotArm_Config *config = &arm_configs[which];

    // 移动所有标记为可移动的节点（顺序移动）
    for (node i = node_0; i < node_MAX; i++) {
        // 检查节点是否可移动
        if (config->movable_nodes[i] == 0) {
            continue; // 跳过不可移动的节点
        }

        // 使用平滑移动函数设置每个节点角度
        int result = BotArm_SmoothSetAngle(P_BotArm, i, angles[i]);
        if (result != 0) {
            return result; // 某个节点设置失败，返回错误
        }
    }

    return 0; // 所有可移动节点设置成功
}

/**
 * @brief 抓手控制实现
 * @param P_BotArm 机械臂设备指针
 * @param state 抓手状态（打开或闭合）
 * @return 成功返回0，失败返回错误码
 */
static int BotArm_Claw(struct BotArm *P_BotArm, claw_state state)
{
    // 参数验证
    if (P_BotArm == NULL) {
        return -1;
    }

    int which = P_BotArm->which;

    if (which < 0 || which >= botarm_MAX) {
        return -2; // 无效的机械臂标识
    }

    // 获取该机械臂的配置
    BotArm_Config *config = &arm_configs[which];

    // 根据状态确定角度
    float angle = (state == claw_open) ? config->open_angle : config->close_angle;

    // 设置抓手节点角度
    return PCA9685_SetServoAngle(config->claw_node, angle);
}

/**
 * @brief 移动关节节点实现（不包含抓手）
 * @param P_BotArm 机械臂设备指针
 * @param angles 角度数组，包含node_0到node_4的角度
 * @return 成功返回0，失败返回错误码
 */
static int BotArm_MoveJoints(struct BotArm *P_BotArm, float angles[])
{
    // 参数验证
    if (P_BotArm == NULL) {
        return -1;
    }

    if (angles == NULL) {
        return -2; // 角度数组为空
    }

    // 关节角度调整逻辑
    if (angles[2] < 35.0f) {
        //关节3的旋转角度限制
        angles[2] = 35.0f;
        angles[3] = 90.0f;
        angles[4] = 90.0f;
    }

    // 使用协调移动，只移动 node_0 到 node_4
    return BotArm_CoordinatedMove(P_BotArm, angles, 5);
}

/**
 * @brief 移动所有可移动节点实现
 * @param P_BotArm 机械臂设备指针
 * @param angles 角度数组，包含所有节点的角度
 * @return 成功返回0，失败返回错误码
 */
static int BotArm_MoveTo(struct BotArm *P_BotArm, float angles[])
{
    // 参数验证
    if (P_BotArm == NULL) {
        return -1;
    }

    if (angles == NULL) {
        return -2; // 角度数组为空
    }

    int which = P_BotArm->which;

    if (which < 0 || which >= botarm_MAX) {
        return -3; // 无效的机械臂标识
    }

    // 获取该机械臂的配置
    BotArm_Config *config = &arm_configs[which];

    // 移动所有标记为可移动的节点
    for (node i = node_0; i < node_MAX; i++) {
        // 检查节点是否可移动
        if (config->movable_nodes[i] == 0) {
            continue; // 跳过不可移动的节点
        }

        int result = BotArm_SetAngle(P_BotArm, i, angles[i]);
        if (result != 0) {
            return result; // 某个节点设置失败，返回错误
        }
    }

    return 0; // 所有可移动节点设置成功
}

// ============================================================================
// 机械臂设备定义 - 每个实例绑定相同的函数集
// ============================================================================

static BotArm BotArmDevices[] = {
    // botarm_0
    {
        botarm_0, // 机械臂标识
        {90.0f, 60.0f, 35.0f, 100.0f, 40.0f, 0}, // 初始化角度记录
        BotArm_Init, // 初始化函数
        BotArm_Claw, // 抓手控制函数
        BotArm_SetAngle, // 设置角度函数
        BotArm_MoveTo, // 移动所有可移动节点函数
        BotArm_MoveJoints, // 移动关节函数
        BotArm_SmoothSetAngle, // 平滑设置角度函数
        BotArm_CoordinatedMove, // 协调移动函数
        BotArm_SmoothMoveTo, // 平滑移动所有可移动节点函数
    },
};

// ============================================================================
// 公共API函数
// ============================================================================

/**
 * @brief 获取指定机械臂设备
 * @param which 机械臂标识
 * @return 成功返回机械臂指针，失败返回NULL
 */
P_BotArm GetBotArmDevice(int which)
{
    if (which >= botarm_0 && which < botarm_MAX) {
        return &BotArmDevices[which];
    }
    return NULL; // 无效的机械臂标识
}

/**
 * @brief 机械臂测试函数 - 演示如何使用API
 */
void BotArm_Test(void)
{
    // 获取机械臂设备指针
    P_BotArm arm0 = GetBotArmDevice(botarm_0);
    if (arm0 != NULL) {
        // 需要传递设备指针
        arm0->Init(arm0); // 初始化机械臂0

        // 使用平滑移动设置单个节点
        arm0->smooth_set_angle(arm0, node_0, 90.0f);

        arm0->claw_set(arm0, claw_open); // 打开抓手
        arm0->claw_set(arm0, claw_close); // 闭合抓手

        // 移动关节（使用协调平滑移动）
        float joint_angles[] = {90.0f, 45.0f, 35.0f, 60.0f, 40.0f}; // 只有5个关节
        arm0->move_joints(arm0, joint_angles);

        // 移动所有可移动节点（使用协调平滑移动）
        float all_angles[] = {90.0f, 45.0f, 35.0f, 60.0f, 40.0f, 999.0f}; // node_5的值会被忽略
        arm0->move_to(arm0, all_angles);

        // 使用新的平滑移动函数（顺序移动）
        arm0->smooth_move_to(arm0, all_angles);
    }
}