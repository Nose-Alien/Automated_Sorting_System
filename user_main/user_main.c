/**
  * @file user_main.c
  * @brief 用户主程序的入口文件
  * @author sleet
  * @date 2025/9/30
  */

#include "user_main.h"
#include "k230_uart_dma.h"
#include "conveyor_device.h"

static uint32_t TIM4_tick_ms = 0; // 静态变量，跟踪时间滴答
P_BotArm arm0;
P_Conveyor_Device conveyor;
float joint_angles_init[] = {90.0f, 60.0f, 35.0f, 100.0f, 180-140.0f};// {90.0f, 50.0f, 35.0f, 100.0f, 140.0f};初始姿态
// float joint_angles[] = {90.0f, 80.0f, 35.0f, 80.0f, 180-140.0f};// {90.0f, 80.0f, 35.0f, 80.0f, 180-140.0f};夹取姿态
/**
  * @brief 主程序入口函数
  * @return int
  */
int user_main()
{
    main_int();
    // arm0->set_angle(arm0, node_4,140);
    while (1) {
        BotArm_ClawPosture();
        arm0->claw_set(arm0,claw_close);
        delay_ms(2000);
        arm0->claw_set(arm0, claw_open);
        delay_ms(2000);
        // conveyor->Forward(conveyor, CONVEYOR_DEFAULT_SPEED);
        // if (Apple == 1 || Strawberry == 1 || Watermelon == 1) {
        //     conveyor->Stop(conveyor);
        //     arm0->claw_set(arm0, claw_open);
        //     delay_ms(2000);
        //     Apple = 0, Strawberry = 0, Watermelon = 0;
        // }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim4) {
        // 判断中断源是否为定时器htim2
        TIM4_tick_ms++; // 每次中断时增加计数s
        if (TIM4_tick_ms % 5 == 0) {
        }
        // 每500毫秒调用一次
        if (TIM4_tick_ms % 500 == 0) {
            HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        }
        if (TIM4_tick_ms % 1000 == 0) {
            // arm0->claw_set(arm0,claw_open);
        }
        if (TIM4_tick_ms % 2000 == 0) {
            // arm0->claw_set(arm0,claw_close);
            TIM4_tick_ms = 0; // 重置计时器计数
        }
    }
}

// void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
// {
//     if (huart== &huart2) {
//         HAL_UART_Transmit_DMA(&huart2, data, Size);
//         HAL_UARTEx_ReceiveToIdle_DMA(&huart2, data, sizeof(data));
//         __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
//     }
// }
void BotArm_ClawPosture(void)
{
    // {90.0f, 80.0f, 35.0f, 80.0f, 180-140.0f};
    arm0->set_angle(arm0, node_4,180-140);
    arm0->set_angle(arm0, node_3,80);
    arm0->set_angle(arm0, node_2,35);
    arm0->set_angle(arm0, node_1,77.5);
    arm0->set_angle(arm0, node_0,90);

}

void main_int()
{
    delay_init(72); //时钟频率
    RetargetInit(&huart1);
    K230_urat_dma_Init();
    HAL_TIM_Base_Start_IT(&htim4); // 启动定时器并使能中断
    conveyor = Conveyor_Device_Create();
    conveyor->Init(conveyor);
    arm0 = GetBotArmDevice(botarm_0);
    arm0->Init(arm0);
    arm0->move_joints(arm0, joint_angles_init);
    arm0->claw_set(arm0, claw_open);
    delay_ms(2000);
}