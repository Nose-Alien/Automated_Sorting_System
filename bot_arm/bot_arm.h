#ifndef BOT_ARM_H
#define BOT_ARM_H

#include "main.h"

// 定义机械臂节点
typedef enum
{
    node_0,
    node_1,
    node_2,
    node_3,
    node_4,
    node_5,
} node;

// 定义抓手状态
typedef enum
{
    claw_open,
    claw_close
} claw_state;

void BotArm_Init(void);// 机械臂初始化
void BotArm_Node(node Channel, float Angle);// 控制机械臂某个节点的角度
claw_state BotArm_Claw(claw_state state);// 控制抓手的开合

#endif //BOT_ARM_H
