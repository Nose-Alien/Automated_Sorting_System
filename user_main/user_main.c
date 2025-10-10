/**
  * @file user_main.c
  * @brief 用户主程序的入口文件
  * @author sleet
  * @date 2025/9/30
  */

#include "user_main.h"
#include "conveyor_device.h"


static uint32_t TIM4_tick_ms = 0; // 静态变量，跟踪时间滴答
P_BotArm arm0;
P_Conveyor_Device conveyor;
float joint_angles[] = {90.0f, 30.0f, 90.0f, 0.0f, 90.0f};

/**
  * @brief 主程序入口函数
  * @return int
  */
int user_main()
{
    delay_init(72); //时钟频率
    HAL_TIM_Base_Start_IT(&htim4); // 启动定时器并使能中断
    arm0 = GetBotArmDevice(botarm_0);
    conveyor = Conveyor_Device_Create();
    arm0->Init(arm0);
    conveyor->Init(conveyor);
    arm0->move_joints(arm0, joint_angles);
    while (1) {
        conveyor->Forward(conveyor, CONVEYOR_DEFAULT_SPEED);
        delay_ms(5000);
        // conveyor->Stop(conveyor);
        // delay_ms(5000);
        conveyor->Backward(conveyor, CONVEYOR_DEFAULT_SPEED);
        delay_ms(5000);
        // arm0->claw_set(claw_open);
        // arm0->move_joints(joint_angles);
        // delay_ms(1000);
        // arm0->claw_set(claw_close);
        // delay_ms(1000);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim4) {
        // 判断中断源是否为定时器htim2
        TIM4_tick_ms++; // 每次中断时增加计数
        // 每5毫秒调用一次
        if (TIM4_tick_ms % 5 == 0) {
        }
        // 每500毫秒调用一次
        if (TIM4_tick_ms % 500 == 0) {
            HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        }
        // 每1000毫秒（1秒）调用一次
        if (TIM4_tick_ms % 1000 == 0) {
            TIM4_tick_ms = 0; // 重置计时器计数
        }
    }
}

