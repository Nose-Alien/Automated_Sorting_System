/**
  * @file user_main.c
  * @brief 用户主程序的入口文件
  * @author Nose_Alien
  * @date 2025/9/30
  */

#include "user_main.h"
#include "k230_uart_dma.h"
#include "conveyor_device.h"
static uint32_t TIM4_tick_ms = 0; // 静态变量，跟踪时间滴答
P_BotArm arm0;
P_Conveyor_Device conveyor;

// 定义几个常用姿态
float joint_angles_grasp_init[] = {90.0f, 70.0f, 35.0f, 100.0f, 40.0f};     // 夹取初始姿态{90.0f, 70.0f, 35.0f, 100.0f, 40.0f};
float joint_angles_grasp[] = {90.0f, 80.0f, 35.0f, 80.0f, 40.0f};      // 夹取姿态90.0f, 80.0f, 35.0f, 80.0f, 40.0f};
float joint_angles_place_init[] = {150.0f, 70.0f, 35.0f, 100.0f, 40.0f};      // 放置初始姿态{150.0f, 70.0f, 35.0f, 100.0f, 40.0f};
float joint_angles_place[] = {150.0f, 110.0f, 35.0f, 100.0f, 40.0f};      // 放置姿态{150.0f, 110.0f, 35.0f, 100.0f, 40.0f};


/**
  * @brief 主程序入口函数
  * @return int
  */
int user_main()
{
    user_main_int();
    delay_ms(2000);
    while (1) {
        // conveyor->Forward(conveyor, CONVEYOR_DEFAULT_SPEED);
        // bot_arm_Action_group();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim4) {
        // 判断中断源是否为定时器htim2
        TIM4_tick_ms++; // 每次中断时增加计数
        if (TIM4_tick_ms % 5 == 0) {
        }
        // 每500毫秒调用一次
        if (TIM4_tick_ms % 500 == 0) {
            HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        }

        // 每10秒重置计时器计数
        if (TIM4_tick_ms >= 1000) {
            TIM4_tick_ms = 0;
        }
    }
}


void user_main_int()
{
    delay_init(72); //时钟频率
    RetargetInit(&huart1);
    K230_urat_dma_Init();
    HAL_TIM_Base_Start_IT(&htim4); // 启动定时器并使能中断
    conveyor = Conveyor_Device_Create();
    conveyor->Init(conveyor);
    arm0 = GetBotArmDevice(botarm_0);
    arm0->Init(arm0);
    // 初始化角度记录
    for (int i = 0; i <= 4; i++) {
        arm0->LastAngl[i] = joint_angles_place_init[i];
    }
    arm0->claw_set(arm0, claw_open);
    arm0->smooth_move_to(arm0, joint_angles_place_init);
}
void bot_arm_Action_group(void)
{
    arm0->smooth_move_to(arm0, joint_angles_grasp_init);
    delay_ms(1000);
    arm0->smooth_move_to(arm0, joint_angles_grasp);
    delay_ms(1000);

    arm0->claw_set(arm0, claw_close);
    delay_ms(1000);
    // 返回到初始姿态
    arm0->smooth_move_to(arm0, joint_angles_grasp_init);
    delay_ms(1000);

    arm0->smooth_move_to(arm0, joint_angles_place);
    delay_ms(1000);

    arm0->claw_set(arm0, claw_open);
    delay_ms(1000);
    // 返回到初始姿态
    arm0->smooth_move_to(arm0, joint_angles_place_init);
    delay_ms(1000);
}