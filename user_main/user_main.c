/**
  * @file user_main.c
  * @brief 用户主程序的入口文件
  * @author sleet
  * @date 2025/9/30
  */

#include "user_main.h"
#include "main.h"

static uint32_t TIM4_tick_ms = 0; // 静态变量，跟踪时间滴答
uint8_t ActiveServo;
/**
  * @brief 主程序入口函数
  * @return int
  */
int user_main()
{
    delay_init(72); //时钟频率
    PCA9685_Init(&hi2c1);
    // HAL_TIM_Base_Start_IT(&htim2);  // 启动定时器并使能中断
    HAL_TIM_Base_Start_IT(&htim4); // 启动定时器并使能中断
    BotArm_Init();
    while (1) {

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

