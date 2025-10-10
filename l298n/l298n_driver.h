/**
  * @file l298n_driver.h
  * @brief L298N底层驱动程序
  * @author Nose_Alien
  * @date 2025/10/10
  */

#ifndef L298N_DRIVER_H
#define L298N_DRIVER_H

#ifdef __cplusplus
extern "C" {

#endif

#include "main.h"

// ============================================================================
// L298N 硬件配置 - 使用预处理宏定义
// ============================================================================

// AIN1 引脚配置 - 控制电机方向
#define L298N_AIN1_PORT          GPIOB
#define L298N_AIN1_PIN           GPIO_PIN_13

// AIN2 引脚配置 - 控制电机方向
#define L298N_AIN2_PORT          GPIOB
#define L298N_AIN2_PIN           GPIO_PIN_12

// PWM 定时器配置 - 控制电机速度
#define L298N_PWM_TIMER          &htim1
#define L298N_PWM_CHANNEL        TIM_CHANNEL_1

// PWM 参数配置
#define L298N_PWM_MAX_VALUE      20000    // PWM定时器的ARR值
#define L298N_PWM_RESOLUTION     200      // 速度转换分辨率

// ============================================================================
// L298N驱动结构体
// ============================================================================

typedef struct L298N_Driver {
    // 引脚定义 - 使用宏定义的值
    GPIO_TypeDef *AIN1_Port;
    uint16_t AIN1_Pin;
    GPIO_TypeDef *AIN2_Port;
    uint16_t AIN2_Pin;

    // PWM 定时器 - 使用宏定义的值
    TIM_HandleTypeDef *PWM_Timer;
    uint32_t PWM_Channel;
} L298N_Driver, *P_L298N_Driver;

// ============================================================================
// 驱动函数声明
// ============================================================================

// 创建L298N驱动实例 (使用预定义的宏配置)
P_L298N_Driver L298N_Driver_Create(void);

// 使用自定义配置创建L298N驱动实例
P_L298N_Driver L298N_Driver_Create_Custom(GPIO_TypeDef *AIN1_Port, uint16_t AIN1_Pin,
                                          GPIO_TypeDef *AIN2_Port, uint16_t AIN2_Pin,
                                          TIM_HandleTypeDef *PWM_Timer, uint32_t PWM_Channel);

// 销毁L298N驱动实例
void L298N_Driver_Destroy(P_L298N_Driver driver);

// 底层驱动函数
int L298N_Driver_Init(P_L298N_Driver driver);

int L298N_Driver_Forward(P_L298N_Driver driver, int dutyRatio);

int L298N_Driver_Backward(P_L298N_Driver driver, int dutyRatio);

int L298N_Driver_Stop(P_L298N_Driver driver);

#ifdef __cplusplus
}
#endif

#endif
