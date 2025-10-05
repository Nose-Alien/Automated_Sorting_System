#include "bot_arm.h"

// 初始化机械臂状态
void BotArm_Init(void)
{
    // 初始化抓手状态为打开
    BotArm_Claw(claw_open);
    // 初始化节点角度
    BotArm_Node(node_0, 90);
    delay_ms(300);
    BotArm_Node(node_1, 90);
    delay_ms(300);
    BotArm_Node(node_2, 90);
    delay_ms(300);
    BotArm_Node(node_3, 90);
    delay_ms(300);
    BotArm_Node(node_4, 90);
    delay_ms(300);
}

// 控制抓手的开合
claw_state BotArm_Claw(claw_state state)
{
    if (state == claw_open) {
        PCA9685_SetServoAngle(node_5, 50);
    } else if (state == claw_close) {
        PCA9685_SetServoAngle(node_5, 100);
    }
    return state;
}

// 控制机械臂各节点的角度
void BotArm_Node(node Channel, float Angle)
{
    PCA9685_SetServoAngle(Channel, Angle);
}

