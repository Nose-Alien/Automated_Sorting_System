#include "bot_arm.h"
#include "pca9685.h"

// 机械臂配置结构体 - 存储每个机械臂的特定配置参数
typedef struct {
    node claw_node;                    // 抓手对应的节点
    float open_angle;                  // 抓手打开角度
    float close_angle;                 // 抓手闭合角度
    float angle_limits[node_MAX][2];   // 每个节点的角度限制 [最小值, 最大值]
    uint8_t movable_nodes[node_MAX];   // 可移动节点标记 (1=可移动, 0=不可移动)
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
        {1, 1, 1, 1, 1, 0} // 可移动节点标记：node_5不可通过move_to移动
    },
    {
        // botarm_1 配置 - 可以有不同的参数
        node_5,
        40.0f,
        110.0f,
        {
            {0, 160}, // node_0 角度范围不同
            {0, 170}, // node_1 角度范围不同
            {0, 150}, // node_2 角度范围不同
            {0, 180}, // node_3 角度范围
            {0, 180}, // node_4 角度范围
            {0, 180} // node_5 角度范围
        },
        {1, 1, 1, 1, 1, 0} // node_5 不可移动
    },
    // 其他机械臂配置可以继续添加...
    {node_5, 50.0f, 100.0f, {{0, 180}, {0, 180}, {0, 180}, {0, 180}, {0, 180}, {0, 180}}, {1, 1, 1, 1, 1, 0}},
    {node_5, 50.0f, 100.0f, {{0, 180}, {0, 180}, {0, 180}, {0, 180}, {0, 180}, {0, 180}}, {1, 1, 1, 1, 1, 0}},
    {node_5, 50.0f, 100.0f, {{0, 180}, {0, 180}, {0, 180}, {0, 180}, {0, 180}, {0, 180}}, {1, 1, 1, 1, 1, 0}}
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

    return 0; // 设置成功
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
    return BotArm_SetAngle(P_BotArm, config->claw_node, angle);
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
    if (angles[2] <= 35.0f) {
        //关节3的旋转角度限制
        angles[2] = 35.0f;
        angles[3] = 90.0f;
        angles[4] = 90.0f;
    }

    angles[4] = 180.0f - angles[4]; //因结构，将关节5的旋转方向反转

    // 只移动 node_0 到 node_4（关节节点），跳过 node_5（抓手）
    for (node i = node_0; i < node_5; i++) {
        int result = BotArm_SetAngle(P_BotArm, i, angles[i]);
        if (result != 0) {
            return result; // 某个节点设置失败，返回错误
        }
    }

    return 0; // 所有关节设置成功
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
        botarm_0,        // 机械臂标识
        BotArm_Init,     // 初始化函数
        BotArm_Claw,     // 抓手控制函数
        BotArm_SetAngle, // 设置角度函数
        BotArm_MoveTo,   // 移动所有可移动节点函数
        BotArm_MoveJoints // 移动关节函数
    },
    // botarm_1
    {
        botarm_1,
        BotArm_Init,
        BotArm_Claw,
        BotArm_SetAngle,
        BotArm_MoveTo,
        BotArm_MoveJoints
    },
    // botarm_2
    {
        botarm_2,
        BotArm_Init,
        BotArm_Claw,
        BotArm_SetAngle,
        BotArm_MoveTo,
        BotArm_MoveJoints
    },
    // botarm_3
    {
        botarm_3,
        BotArm_Init,
        BotArm_Claw,
        BotArm_SetAngle,
        BotArm_MoveTo,
        BotArm_MoveJoints
    },
    // botarm_4
    {
        botarm_4,
        BotArm_Init,
        BotArm_Claw,
        BotArm_SetAngle,
        BotArm_MoveTo,
        BotArm_MoveJoints
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
        arm0->Init(arm0);                              // 初始化机械臂0
        arm0->set_angle(arm0, node_0, 90.0f);          // 设置节点0角度为90度
        arm0->claw_set(arm0, claw_open);               // 打开抓手
        arm0->claw_set(arm0, claw_close);              // 闭合抓手

        // 移动关节（不包含抓手）
        float joint_angles[] = {90.0f, 45.0f, 30.0f, 60.0f, 0.0f}; // 只有5个关节
        arm0->move_joints(arm0, joint_angles);

        // 移动所有可移动节点（node_5不会被移动）
        float all_angles[] = {90.0f, 45.0f, 30.0f, 60.0f, 0.0f, 999.0f}; // node_5的值会被忽略
        arm0->move_to(arm0, all_angles);
    }

    // 使用另一个机械臂
    P_BotArm arm1 = GetBotArmDevice(botarm_1);
    if (arm1 != NULL) {
        arm1->Init(arm1);
        arm1->set_angle(arm1, node_1, 45.0f);          // 设置机械臂1的节点1角度为45度
        arm1->claw_set(arm1, claw_open);               // 打开机械臂1的抓手

        float joints[] = {80.0f, 35.0f, 40.0f, 70.0f, 10.0f};
        arm1->move_joints(arm1, joints);               // 移动机械臂1的关节

        arm1->claw_set(arm1, claw_close);              // 闭合机械臂1的抓手
    }
}