#include "bot_arm.h"


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
// 内部实现函数 - 这些函数接收which参数，知道操作的是哪个机械臂
// ============================================================================

/**
 * @brief 机械臂初始化实现
 * @param which 机械臂标识
 * @return 成功返回0，失败返回错误码
 */
static int BotArm_Init_Impl(int which)
{
    // 初始化PCA9685等底层硬件
    PCA9685_Init(&hi2c1);

    // 这里可以根据which进行特定机械臂的初始化
    // 例如：复位到初始位置、读取校准参数等

    return 0; // 初始化成功
}

/**
 * @brief 设置节点角度实现
 * @param which 机械臂标识
 * @param Channel 节点编号
 * @param angle 目标角度
 * @return 成功返回0，失败返回错误码
 */
static int BotArm_SetAngle_Impl(int which, node Channel, float angle)
{
    // 参数验证
    if (which < 0 || which >= botarm_MAX) {
        return -1; // 无效的机械臂标识
    }

    if (Channel < node_0 || Channel >= node_MAX) {
        return -2; // 无效的节点编号
    }

    // 获取该机械臂的配置
    BotArm_Config *config = &arm_configs[which];

    // 角度范围检查
    if (angle < config->angle_limits[Channel][0] ||
        angle > config->angle_limits[Channel][1]) {
        return -3; // 角度超出限制
    }

    // 调用底层PCA9685驱动设置角度
    PCA9685_SetServoAngle(Channel, angle);
    // delay_ms(1000);

    return 0; // 设置成功
}

/**
 * @brief 抓手控制实现
 * @param which 机械臂标识
 * @param state 抓手状态（打开或闭合）
 * @return 成功返回0，失败返回错误码
 */
static int BotArm_Claw_Impl(int which, claw_state state)
{
    // 参数验证
    if (which < 0 || which >= botarm_MAX) {
        return -1; // 无效的机械臂标识
    }

    // 获取该机械臂的配置
    BotArm_Config *config = &arm_configs[which];

    // 根据状态确定角度
    float angle = (state == claw_open) ? config->open_angle : config->close_angle;

    // 设置抓手节点角度
    return BotArm_SetAngle_Impl(which, config->claw_node, angle);
}

/**
 * @brief 移动关节节点实现（不包含抓手）
 * @param which 机械臂标识
 * @param angles 角度数组，包含node_0到node_4的角度
 * @return 成功返回0，失败返回错误码
 */
static int BotArm_MoveJoints_Impl(int which, float angles[])
{
    // 参数验证
    if (which < 0 || which >= botarm_MAX) {
        return -1; // 无效的机械臂标识
    }

    if (angles == NULL) {
        return -2; // 角度数组为空
    }

    if (angles[2] <= 35.0f) {
        //关节3的旋转角度限制
        angles[2] = 35.0f;
        angles[3] = 90.0f;
        angles[4] = 90.0f;
    }

    angles[4] = 180.0f - angles[4]; //因结构，将关节5的旋转方向反转

    // 只移动 node_0 到 node_4（关节节点），跳过 node_5（抓手）
    for (node i = node_0; i < node_5; i++) {
        int result = BotArm_SetAngle_Impl(which, i, angles[i]);
        if (result != 0) {
            return result; // 某个节点设置失败，返回错误
        }
    }

    return 0; // 所有关节设置成功
}

/**
 * @brief 移动所有可移动节点实现
 * @param which 机械臂标识
 * @param angles 角度数组，包含所有节点的角度
 * @return 成功返回0，失败返回错误码
 */
static int BotArm_MoveTo_Impl(int which, float angles[])
{
    // 参数验证
    if (which < 0 || which >= botarm_MAX) {
        return -1; // 无效的机械臂标识
    }

    if (angles == NULL) {
        return -2; // 角度数组为空
    }

    // 获取该机械臂的配置
    BotArm_Config *config = &arm_configs[which];

    // 移动所有标记为可移动的节点
    for (node i = node_0; i < node_MAX; i++) {
        // 检查节点是否可移动
        if (config->movable_nodes[i] == 0) {
            continue; // 跳过不可移动的节点
        }

        int result = BotArm_SetAngle_Impl(which, i, angles[i]);
        if (result != 0) {
            return result; // 某个节点设置失败，返回错误
        }
    }

    return 0; // 所有可移动节点设置成功
}

// ============================================================================
// 实例专用函数 - 每个机械臂实例有自己的函数，这些函数硬编码了which参数
// ============================================================================

// botarm_0 专用函数
static int BotArm0_Init(void) { return BotArm_Init_Impl(botarm_0); }
static int BotArm0_Claw(claw_state state) { return BotArm_Claw_Impl(botarm_0, state); }
static int BotArm0_SetAngle(node ch, float angle) { return BotArm_SetAngle_Impl(botarm_0, ch, angle); }
static int BotArm0_MoveTo(float angles[]) { return BotArm_MoveTo_Impl(botarm_0, angles); }
static int BotArm0_MoveJoints(float angles[]) { return BotArm_MoveJoints_Impl(botarm_0, angles); }

// botarm_1 专用函数
static int BotArm1_Init(void) { return BotArm_Init_Impl(botarm_1); }
static int BotArm1_Claw(claw_state state) { return BotArm_Claw_Impl(botarm_1, state); }
static int BotArm1_SetAngle(node ch, float angle) { return BotArm_SetAngle_Impl(botarm_1, ch, angle); }
static int BotArm1_MoveTo(float angles[]) { return BotArm_MoveTo_Impl(botarm_1, angles); }
static int BotArm1_MoveJoints(float angles[]) { return BotArm_MoveJoints_Impl(botarm_1, angles); }

// botarm_2 专用函数
static int BotArm2_Init(void) { return BotArm_Init_Impl(botarm_2); }
static int BotArm2_Claw(claw_state state) { return BotArm_Claw_Impl(botarm_2, state); }
static int BotArm2_SetAngle(node ch, float angle) { return BotArm_SetAngle_Impl(botarm_2, ch, angle); }
static int BotArm2_MoveTo(float angles[]) { return BotArm_MoveTo_Impl(botarm_2, angles); }
static int BotArm2_MoveJoints(float angles[]) { return BotArm_MoveJoints_Impl(botarm_2, angles); }

// botarm_3 专用函数
static int BotArm3_Init(void) { return BotArm_Init_Impl(botarm_3); }
static int BotArm3_Claw(claw_state state) { return BotArm_Claw_Impl(botarm_3, state); }
static int BotArm3_SetAngle(node ch, float angle) { return BotArm_SetAngle_Impl(botarm_3, ch, angle); }
static int BotArm3_MoveTo(float angles[]) { return BotArm_MoveTo_Impl(botarm_3, angles); }
static int BotArm3_MoveJoints(float angles[]) { return BotArm_MoveJoints_Impl(botarm_3, angles); }

// botarm_4 专用函数
static int BotArm4_Init(void) { return BotArm_Init_Impl(botarm_4); }
static int BotArm4_Claw(claw_state state) { return BotArm_Claw_Impl(botarm_4, state); }
static int BotArm4_SetAngle(node ch, float angle) { return BotArm_SetAngle_Impl(botarm_4, ch, angle); }
static int BotArm4_MoveTo(float angles[]) { return BotArm_MoveTo_Impl(botarm_4, angles); }
static int BotArm4_MoveJoints(float angles[]) { return BotArm_MoveJoints_Impl(botarm_4, angles); }

// ============================================================================
// 机械臂设备定义 - 每个实例绑定自己的函数集
// ============================================================================

static BotArm BotArmDevices[] = {
    // botarm_0 - 绑定botarm_0专用函数
    {
        botarm_0, // 机械臂标识
        BotArm0_Init, // 初始化函数
        BotArm0_Claw, // 抓手控制函数
        BotArm0_SetAngle, // 设置角度函数
        BotArm0_MoveTo, // 移动所有可移动节点函数
        BotArm0_MoveJoints // 移动关节函数
    },
    // botarm_1 - 绑定botarm_1专用函数
    {
        botarm_1,
        BotArm1_Init,
        BotArm1_Claw,
        BotArm1_SetAngle,
        BotArm1_MoveTo,
        BotArm1_MoveJoints
    },
    // botarm_2 - 绑定botarm_2专用函数
    {
        botarm_2,
        BotArm2_Init,
        BotArm2_Claw,
        BotArm2_SetAngle,
        BotArm2_MoveTo,
        BotArm2_MoveJoints
    },
    // botarm_3 - 绑定botarm_3专用函数
    {
        botarm_3,
        BotArm3_Init,
        BotArm3_Claw,
        BotArm3_SetAngle,
        BotArm3_MoveTo,
        BotArm3_MoveJoints
    },
    // botarm_4 - 绑定botarm_4专用函数
    {
        botarm_4,
        BotArm4_Init,
        BotArm4_Claw,
        BotArm4_SetAngle,
        BotArm4_MoveTo,
        BotArm4_MoveJoints
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
    // 方法1：直接使用设备指针（面向对象风格）
    P_BotArm arm0 = GetBotArmDevice(botarm_0);
    if (arm0 != NULL) {
        arm0->Init(); // 初始化机械臂0
        arm0->set_angle(node_0, 90.0f); // 设置节点0角度为90度
        arm0->claw_set(claw_open); // 打开抓手
        arm0->claw_set(claw_close); // 闭合抓手

        // 移动关节（不包含抓手）
        float joint_angles[] = {90.0f, 45.0f, 30.0f, 60.0f, 0.0f}; // 只有5个关节
        arm0->move_joints(joint_angles);

        // 移动所有可移动节点（node_5不会被移动）
        float all_angles[] = {90.0f, 45.0f, 30.0f, 60.0f, 0.0f, 999.0f}; // node_5的值会被忽略
        arm0->move_to(all_angles);
    }

    // 方法2：使用简化宏（函数式风格）
    ARM_INIT(botarm_1); // 初始化机械臂1
    ARM_ANGLE(botarm_1, node_1, 45.0f); // 设置机械臂1的节点1角度为45度
    ARM_CLAW(botarm_1, claw_open); // 打开机械臂1的抓手

    float joints[] = {80.0f, 35.0f, 40.0f, 70.0f, 10.0f};
    ARM_MOVE_JOINTS(botarm_1, joints); // 移动机械臂1的关节

    // 演示不同机械臂可以有不同的配置
    P_BotArm arm1 = GetBotArmDevice(botarm_1);
    arm1->claw_set(claw_close); // 闭合机械臂1的抓手
}
