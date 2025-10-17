/**
  * @file bot_arm.h
  * @brief 机械臂接口定义与类型声明
  * @author sleet
  * @date 2025/10/5
  */

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

// 机械臂操作接口结构体
typedef struct BotArm {
    int which;  // 机械臂标识，用于区分不同机械臂实例
    float LastAngl[6]; // 记录上次夹取角度

    // 函数指针 - 需要传递设备指针
    int (*Init)(struct BotArm *P_BotArm);                       // 初始化函数
    int (*claw_set)(struct BotArm *P_BotArm, claw_state state); // 抓手控制函数
    int (*set_angle)(struct BotArm *P_BotArm, node Channel, float Angle); // 设置单个节点角度
    int (*move_to)(struct BotArm *P_BotArm, float angles[]);    // 移动所有可移动节点
    int (*move_joints)(struct BotArm *P_BotArm, float angles[]); // 只移动关节节点（不包含抓手）
    int (*smooth_set_angle)(struct BotArm *P_BotArm, node Channel, float Angle); // 平滑设置单个节点角度
    int (*coordinated_move)(struct BotArm *P_BotArm, float angles[], uint8_t num_nodes); // 协调移动多个节点
} BotArm, *P_BotArm;

// 获取指定机械臂设备
P_BotArm GetBotArmDevice(int which);

// 测试函数
void BotArm_Test(void);

#endif // BOT_ARM_H