#ifndef BOT_ARM_H
#define BOT_ARM_H

#include "main.h"

// 机械臂标识枚举
enum {
    botarm_0,  // 机械臂0
    botarm_1,  // 机械臂1
    botarm_2,  // 机械臂2
    botarm_3,  // 机械臂3
    botarm_4,  // 机械臂4
    botarm_MAX, // 机械臂最大数量
};

// 机械臂节点枚举
typedef enum {
    node_0,  // 节点0
    node_1,  // 节点1
    node_2,  // 节点2
    node_3,  // 节点3
    node_4,  // 节点4
    node_5,  // 节点5（抓手）
    node_MAX // 节点最大数量
} node;

// 抓手状态枚举
typedef enum {
    claw_open,   // 抓手打开
    claw_close   // 抓手闭合
} claw_state;

// 机械臂配置结构体 - 存储每个机械臂的特定配置参数
typedef struct {
    node claw_node;                    // 抓手对应的节点
    float open_angle;                  // 抓手打开角度
    float close_angle;                 // 抓手闭合角度
    float angle_limits[node_MAX][2];   // 每个节点的角度限制 [最小值, 最大值]
    uint8_t movable_nodes[node_MAX];   // 可移动节点标记 (1=可移动, 0=不可移动)
} BotArm_Config;

// 机械臂操作接口结构体
typedef struct BotArm {
    int which;  // 机械臂标识，用于区分不同机械臂实例

    // 函数指针 - 不需要传递设备指针，因为每个实例已经绑定了自己的标识
    int (*Init)(void);                                  // 初始化函数
    int (*claw_set)(claw_state state);                  // 抓手控制函数
    int (*set_angle)(node Channel, float Angle);        // 设置单个节点角度
    int (*move_to)(float angles[]);                     // 移动所有可移动节点
    int (*move_joints)(float angles[]);                 // 只移动关节节点（不包含抓手）
} BotArm, *P_BotArm;

// 获取指定机械臂设备
P_BotArm GetBotArmDevice(int which);

// 简化调用宏 - 让API调用更加简洁
#define ARM_INIT(which) GetBotArmDevice(which)->Init()
#define ARM_CLAW(which, state) GetBotArmDevice(which)->claw_set(state)
#define ARM_ANGLE(which, ch, angle) GetBotArmDevice(which)->set_angle(ch, angle)
#define ARM_MOVE_JOINTS(which, angles) GetBotArmDevice(which)->move_joints(angles)
#define ARM_MOVE_TO(which, angles) GetBotArmDevice(which)->move_to(angles)

// 测试函数
void BotArm_Test(void);

#endif // BOT_ARM_H