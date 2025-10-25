/**
  * @file conveyor_device.h
  * @brief 传送带设备抽象层头文件
  * @author sleet
  * @date 2025/10/10
  */

#ifndef CONVEYOR_DEVICE_H
#define CONVEYOR_DEVICE_H

#ifdef __cplusplus
extern "C" {

#endif

#include "main.h"
#include "l298n_driver.h"

// ============================================================================
// 设备配置宏
// ============================================================================

// 设备名称
#define CONVEYOR_DEVICE_NAME     "MainConveyor"

// 默认速度设置
#define CONVEYOR_DEFAULT_SPEED   20        // 默认速度 20%
#define CONVEYOR_MAX_SPEED       100       // 最大速度 100%
#define CONVEYOR_MIN_SPEED       0         // 最小速度 0%

// 安全参数
#define CONVEYOR_SAFETY_DELAY_MS 100       // 安全操作延时(ms)

// ============================================================================
// 设备类型定义
// ============================================================================

// 传送带方向枚举
typedef enum {
    CONVEYOR_FORWARD, // 前进
    CONVEYOR_BACKWARD, // 后退
    CONVEYOR_STOP // 停止
} Conveyor_Direction;

// 传送带设备结构体
typedef struct Conveyor_Device {
    // 底层驱动
    P_L298N_Driver driver;

    // 设备状态
    Conveyor_Direction current_direction;
    int current_speed;
    char device_name[32];

    // 设备方法
    int (*Init)(struct Conveyor_Device *device);

    int (*Forward)(struct Conveyor_Device *device, int dutyRatio);

    int (*Backward)(struct Conveyor_Device *device, int dutyRatio);

    int (*Stop)(struct Conveyor_Device *device);

    int (*GetDirection)(struct Conveyor_Device *device);

    int (*GetSpeed)(struct Conveyor_Device *device);

    const char * (*GetName)(struct Conveyor_Device *device);
} Conveyor_Device, *P_Conveyor_Device;

// ============================================================================
// 设备函数声明
// ============================================================================

// 创建传送带设备实例 (使用预定义的宏配置)
P_Conveyor_Device Conveyor_Device_Create(void);

// 创建带自定义名称的传送带设备
P_Conveyor_Device Conveyor_Device_Create_With_Name(const char *device_name);

// 使用现有驱动创建传送带设备
P_Conveyor_Device Conveyor_Device_Create_With_Driver(P_L298N_Driver driver);

// 销毁传送带设备实例
void Conveyor_Device_Destroy(P_Conveyor_Device device);

// 测试函数
int Conveyor_Device_Test(void);

#ifdef __cplusplus
}
#endif

#endif
