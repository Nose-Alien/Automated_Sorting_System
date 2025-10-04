/*
 * pca9685.h
 *
 *  Created on: 20.01.2019
 *      Author: Mateusz Salamon
 *      mateusz@msalamon.pl
 *
 *      Website: https://msalamon.pl/nigdy-wiecej-multipleksowania-na-gpio!-max7219-w-akcji-cz-3/
 *      GitHub:  https://github.com/lamik/Servos_PWM_STM32_HAL
 */

#ifndef PCA9685_H_
#define PCA9685_H_

//
//  Enable Servo control mode
//
#define PCA9685_SERVO_MODE

#ifdef PCA9685_SERVO_MODE
//
//  Servo min and max values for TURINGY TG9e Servos
//
#define SERVO_MIN  110       // 最小脉宽值
#define SERVO_MAX  500       // 最大脉宽值
#define MIN_ANGLE  0.0       // 最小角度（0°）
#define MAX_ANGLE  180.0     // 最大角度（180°）

#define SERVO_FREQ   50   // 50Hz
#define SERVO_STOP   1500 // 1.5ms = 停止
#define SERVO_360_Forward    1290  // 0.5ms = 正转最快
#define SERVO_360_Reverse    1690 // 2.5ms = 反转最快

#endif

//
//  Adjustable address 0x80 - 0xFE
//
#define PCA9685_ADDRESS  0x80  // 定义PCA9685的I2C地址，从0x80到0xFE可调

//
//  Registers
//
#define PCA9685_SUBADR1  0x2     // 子地址1寄存器
#define PCA9685_SUBADR2  0x3     // 子地址2寄存器
#define PCA9685_SUBADR3  0x4     // 子地址3寄存器

#define PCA9685_MODE1    0x0     // 模式寄存器1
#define PCA9685_PRESCALE 0xFE    // 频率预分频器寄存器

#define PCA9685_LED0_ON_L  0x6    // LED0通道开启时间低字节寄存器
#define PCA9685_LED0_ON_H  0x7    // LED0通道开启时间高字节寄存器
#define PCA9685_LED0_OFF_L 0x8    // LED0通道关闭时间低字节寄存器
#define PCA9685_LED0_OFF_H 0x9    // LED0通道关闭时间高字节寄存器

#define PCA9685_ALLLED_ON_L  0xFA  // 所有LED开启时间低字节寄存器
#define PCA9685_ALLLED_ON_H  0xFB  // 所有LED开启时间高字节寄存器
#define PCA9685_ALLLED_OFF_L 0xFC  // 所有LED关闭时间低字节寄存器
#define PCA9685_ALLLED_OFF_H 0xFD  // 所有LED关闭时间高字节寄存器

#define PCA9685_MODE1_ALCALL_BIT  0    // ALCALL位
typedef enum
{
    PCA9685_MODE1_SUB1_BIT   = 3,  // 子地址1位
    PCA9685_MODE1_SUB2_BIT   = 2,  // 子地址2位
    PCA9685_MODE1_SUB3_BIT   = 1   // 子地址3位
} SubaddressBit;

typedef enum
{
    Stop,
    Forward,
    Reverse
}Servo360;
typedef enum
{
    servo_0,
    servo_1,
    servo_2,
    servo_3,
    servo_4,
    servo_5,
    servo_6,
    servo_7,
    servo_8,
    servo_9,
    servo_10,
    servo_11,

}servo;

#define PCA9685_MODE1_SLEEP_BIT     4    // 睡眠模式位
#define PCA9685_MODE1_AI_BIT        5    // 自动增量模式位
#define PCA9685_MODE1_EXTCLK_BIT    6    // 外部时钟位
#define PCA9685_MODE1_RESTART_BIT   7    // 重启位


typedef enum
{
    PCA9685_OK      = 0,   // 操作成功
    PCA9685_ERROR   = 1    // 操作失败
} PCA9685_STATUS;

// 函数声明部分
PCA9685_STATUS PCA9685_SoftwareReset(void);            // 软件复位函数
PCA9685_STATUS PCA9685_SleepMode(uint8_t Enable);      // 控制睡眠模式的函数
PCA9685_STATUS PCA9685_RestartMode(uint8_t Enable);    // 控制重新启动模式的函数
PCA9685_STATUS PCA9685_AutoIncrement(uint8_t Enable);  // 启用/禁用自动递增模式

#ifndef PCA9685_SERVO_MODE
PCA9685_STATUS PCA9685_SetPwmFrequency(uint16_t Frequency);  // 设置PWM频率（非伺服模式）
#endif

PCA9685_STATUS PCA9685_SetPwm(uint8_t Channel, uint16_t OnTime, uint16_t OffTime);  // 设置PWM值
PCA9685_STATUS PCA9685_SetPin(uint8_t Channel, uint16_t Value, uint8_t Invert);    // 设置引脚输出

#ifdef PCA9685_SERVO_MODE
PCA9685_STATUS PCA9685_SetServoAngle(uint8_t Channel, float Angle);  // 设置伺服电机角度
#endif

PCA9685_STATUS PCA9685_Init(I2C_HandleTypeDef *hi2c);  // 初始化PCA9685，传入I2C句柄
PCA9685_STATUS PCA9685_Test();
uint16_t ServoUsToTicks(uint16_t us, uint16_t freq);
Servo360 Servo360_SetSpeed(uint8_t channel, Servo360 state);

#endif /* PCA9685_H_ */
