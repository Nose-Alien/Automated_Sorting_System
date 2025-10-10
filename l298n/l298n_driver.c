/**
  * @file l298n_driver.c
  * @brief L298N底层驱动程序
  * @author Nose_Alien
  * @date 2025/10/10
  */

#include "l298n_driver.h"
#include <stdlib.h>


// ============================================================================
// 内部函数
// ============================================================================

/**
 * @brief 速度转换函数
 * @param dutyRatio 占空比 (0-100)
 * @return PWM比较值
 */
static int speed_convert(int dutyRatio)
{
    if ((dutyRatio < 0) || (dutyRatio > 100)) return -1;
    return (dutyRatio * L298N_PWM_RESOLUTION); // 使用预定义的分辨率
}

// ============================================================================
// 工厂函数
// ============================================================================

/**
 * @brief 创建L298N驱动实例 (使用预定义的宏配置)
 * @return 成功返回驱动指针，失败返回NULL
 */
P_L298N_Driver L298N_Driver_Create(void)
{
    P_L298N_Driver driver = (P_L298N_Driver) malloc(sizeof(L298N_Driver));
    if (driver == NULL) return NULL;

    // 使用预定义的宏配置硬件参数
    driver->AIN1_Port = L298N_AIN1_PORT;
    driver->AIN1_Pin = L298N_AIN1_PIN;
    driver->AIN2_Port = L298N_AIN2_PORT;
    driver->AIN2_Pin = L298N_AIN2_PIN;
    driver->PWM_Timer = L298N_PWM_TIMER;
    driver->PWM_Channel = L298N_PWM_CHANNEL;

    return driver;
}

/**
 * @brief 使用自定义配置创建L298N驱动实例
 * @param AIN1_Port AIN1 GPIO端口
 * @param AIN1_Pin AIN1 GPIO引脚
 * @param AIN2_Port AIN2 GPIO端口
 * @param AIN2_Pin AIN2 GPIO引脚
 * @param PWM_Timer PWM定时器句柄
 * @param PWM_Channel PWM通道
 * @return 成功返回驱动指针，失败返回NULL
 */
P_L298N_Driver L298N_Driver_Create_Custom(GPIO_TypeDef *AIN1_Port, uint16_t AIN1_Pin,
                                          GPIO_TypeDef *AIN2_Port, uint16_t AIN2_Pin,
                                          TIM_HandleTypeDef *PWM_Timer, uint32_t PWM_Channel)
{
    P_L298N_Driver driver = (P_L298N_Driver) malloc(sizeof(L298N_Driver));
    if (driver == NULL) return NULL;

    // 使用自定义配置
    driver->AIN1_Port = AIN1_Port;
    driver->AIN1_Pin = AIN1_Pin;
    driver->AIN2_Port = AIN2_Port;
    driver->AIN2_Pin = AIN2_Pin;
    driver->PWM_Timer = PWM_Timer;
    driver->PWM_Channel = PWM_Channel;

    return driver;
}

/**
 * @brief 销毁L298N驱动实例
 * @param driver 驱动指针
 */
void L298N_Driver_Destroy(P_L298N_Driver driver)
{
    if (driver != NULL) {
        free(driver);
    }
}

// ============================================================================
// 驱动函数实现
// ============================================================================

/**
 * @brief L298N驱动初始化
 * @param driver 驱动指针
 * @return 成功返回0，失败返回错误码
 */
int L298N_Driver_Init(P_L298N_Driver driver)
{
    if (driver == NULL) return -1;

    // 启动PWM定时器
    HAL_TIM_PWM_Start(driver->PWM_Timer, driver->PWM_Channel);

    // 确保初始状态为停止
    HAL_GPIO_WritePin(driver->AIN1_Port, driver->AIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(driver->AIN2_Port, driver->AIN2_Pin, GPIO_PIN_RESET);
    __HAL_TIM_SET_COMPARE(driver->PWM_Timer, driver->PWM_Channel, 0);

    return 0;
}

/**
 * @brief 电机前进
 * @param driver 驱动指针
 * @param dutyRatio 占空比 (0-100)
 * @return 成功返回0，失败返回错误码
 */
int L298N_Driver_Forward(P_L298N_Driver driver, int dutyRatio)
{
    if (driver == NULL) return -1;
    if ((dutyRatio < 0) || (dutyRatio > 100)) return -2;

    // 设置电机方向为前进
    HAL_GPIO_WritePin(driver->AIN1_Port, driver->AIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(driver->AIN2_Port, driver->AIN2_Pin, GPIO_PIN_SET);
    __HAL_TIM_SET_COMPARE(driver->PWM_Timer, driver->PWM_Channel, speed_convert(dutyRatio));

    return 0;
}

/**
 * @brief 电机后退
 * @param driver 驱动指针
 * @param dutyRatio 占空比 (0-100)
 * @return 成功返回0，失败返回错误码
 */
int L298N_Driver_Backward(P_L298N_Driver driver, int dutyRatio)
{
    if (driver == NULL) return -1;
    if ((dutyRatio < 0) || (dutyRatio > 100)) return -2;

    // 设置电机方向为后退
    HAL_GPIO_WritePin(driver->AIN1_Port, driver->AIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(driver->AIN2_Port, driver->AIN2_Pin, GPIO_PIN_RESET);
    __HAL_TIM_SET_COMPARE(driver->PWM_Timer, driver->PWM_Channel, speed_convert(dutyRatio));

    return 0;
}

/**
 * @brief 电机停止
 * @param driver 驱动指针
 * @return 成功返回0，失败返回错误码
 */
int L298N_Driver_Stop(P_L298N_Driver driver)
{
    if (driver == NULL) return -1;

    // 停止电机
    HAL_GPIO_WritePin(driver->AIN1_Port, driver->AIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(driver->AIN2_Port, driver->AIN2_Pin, GPIO_PIN_RESET);
    __HAL_TIM_SET_COMPARE(driver->PWM_Timer, driver->PWM_Channel, 0);

    return 0;
}
