/**
  * @file conveyor_device.c
  * @brief 传送带设备抽象层
  * @author Nose_Alien
  * @date 2025/10/10
  */

#include "conveyor_device.h"

#include <stdlib.h>
#include <string.h>


// ============================================================================
// 设备方法实现
// ============================================================================

/**
 * @brief 传送带设备初始化
 * @param device 设备指针
 * @return 成功返回0，失败返回错误码
 */
static int Conveyor_Device_Init(P_Conveyor_Device device)
{
    if (device == NULL) return -1;
    if (device->driver == NULL) return -2;

    // 初始化底层驱动
    int result = L298N_Driver_Init(device->driver);
    if (result != 0) return result;

    // 初始化设备状态
    device->current_direction = CONVEYOR_STOP;
    device->current_speed = 0;

    return 0;
}

/**
 * @brief 传送带前进
 * @param device 设备指针
 * @param dutyRatio 占空比 (0-100)
 * @return 成功返回0，失败返回错误码
 */
static int Conveyor_Device_Forward(P_Conveyor_Device device, int dutyRatio)
{
    if (device == NULL) return -1;
    if (device->driver == NULL) return -2;

    // 速度范围检查
    if (dutyRatio < CONVEYOR_MIN_SPEED) dutyRatio = CONVEYOR_MIN_SPEED;
    if (dutyRatio > CONVEYOR_MAX_SPEED) dutyRatio = CONVEYOR_MAX_SPEED;

    // 调用底层驱动
    int result = L298N_Driver_Forward(device->driver, dutyRatio);
    if (result == 0) {
        device->current_direction = CONVEYOR_FORWARD;
        device->current_speed = dutyRatio;
    }

    return result;
}

/**
 * @brief 传送带后退
 * @param device 设备指针
 * @param dutyRatio 占空比 (0-100)
 * @return 成功返回0，失败返回错误码
 */
static int Conveyor_Device_Backward(P_Conveyor_Device device, int dutyRatio)
{
    if (device == NULL) return -1;
    if (device->driver == NULL) return -2;

    // 速度范围检查
    if (dutyRatio < CONVEYOR_MIN_SPEED) dutyRatio = CONVEYOR_MIN_SPEED;
    if (dutyRatio > CONVEYOR_MAX_SPEED) dutyRatio = CONVEYOR_MAX_SPEED;

    // 调用底层驱动
    int result = L298N_Driver_Backward(device->driver, dutyRatio);
    if (result == 0) {
        device->current_direction = CONVEYOR_BACKWARD;
        device->current_speed = dutyRatio;
    }

    return result;
}

/**
 * @brief 传送带停止
 * @param device 设备指针
 * @return 成功返回0，失败返回错误码
 */
static int Conveyor_Device_Stop(P_Conveyor_Device device)
{
    if (device == NULL) return -1;
    if (device->driver == NULL) return -2;

    // 调用底层驱动
    int result = L298N_Driver_Stop(device->driver);
    if (result == 0) {
        device->current_direction = CONVEYOR_STOP;
        device->current_speed = 0;
    }

    return result;
}

/**
 * @brief 获取传送带当前方向
 * @param device 设备指针
 * @return 当前方向
 */
static int Conveyor_Device_GetDirection(P_Conveyor_Device device)
{
    if (device == NULL) return -1;
    return device->current_direction;
}

/**
 * @brief 获取传送带当前速度
 * @param device 设备指针
 * @return 当前速度
 */
static int Conveyor_Device_GetSpeed(P_Conveyor_Device device)
{
    if (device == NULL) return -1;
    return device->current_speed;
}

/**
 * @brief 获取设备名称
 * @param device 设备指针
 * @return 设备名称字符串
 */
static const char *Conveyor_Device_GetName(P_Conveyor_Device device)
{
    if (device == NULL) return "Invalid Device";
    return device->device_name;
}

// ============================================================================
// 工厂函数
// ============================================================================

/**
 * @brief 创建传送带设备实例 (使用预定义的宏配置)
 * @return 成功返回设备指针，失败返回NULL
 */
P_Conveyor_Device Conveyor_Device_Create(void)
{
    // 使用预定义的配置创建底层驱动
    P_L298N_Driver driver = L298N_Driver_Create();
    if (driver == NULL) return NULL;

    // 使用驱动创建设备
    P_Conveyor_Device device = Conveyor_Device_Create_With_Driver(driver);
    if (device == NULL) {
        L298N_Driver_Destroy(driver);
        return NULL;
    }

    // 设置默认设备名称
    strncpy(device->device_name, CONVEYOR_DEVICE_NAME, sizeof(device->device_name) - 1);
    device->device_name[sizeof(device->device_name) - 1] = '\0';

    return device;
}

/**
 * @brief 创建带自定义名称的传送带设备
 * @param device_name 设备名称
 * @return 成功返回设备指针，失败返回NULL
 */
P_Conveyor_Device Conveyor_Device_Create_With_Name(const char *device_name)
{
    P_Conveyor_Device device = Conveyor_Device_Create();
    if (device == NULL) return NULL;

    if (device_name != NULL) {
        strncpy(device->device_name, device_name, sizeof(device->device_name) - 1);
        device->device_name[sizeof(device->device_name) - 1] = '\0';
    }

    return device;
}

/**
 * @brief 使用现有驱动创建传送带设备
 * @param driver 底层驱动指针
 * @return 成功返回设备指针，失败返回NULL
 */
P_Conveyor_Device Conveyor_Device_Create_With_Driver(P_L298N_Driver driver)
{
    if (driver == NULL) return NULL;

    P_Conveyor_Device device = (P_Conveyor_Device) malloc(sizeof(Conveyor_Device));
    if (device == NULL) return NULL;

    // 设置底层驱动
    device->driver = driver;

    // 初始化设备状态
    device->current_direction = CONVEYOR_STOP;
    device->current_speed = 0;
    strncpy(device->device_name, CONVEYOR_DEVICE_NAME, sizeof(device->device_name) - 1);
    device->device_name[sizeof(device->device_name) - 1] = '\0';

    // 绑定设备方法
    device->Init = Conveyor_Device_Init;
    device->Forward = Conveyor_Device_Forward;
    device->Backward = Conveyor_Device_Backward;
    device->Stop = Conveyor_Device_Stop;
    device->GetDirection = Conveyor_Device_GetDirection;
    device->GetSpeed = Conveyor_Device_GetSpeed;
    device->GetName = Conveyor_Device_GetName;

    return device;
}

/**
 * @brief 销毁传送带设备实例
 * @param device 设备指针
 */
void Conveyor_Device_Destroy(P_Conveyor_Device device)
{
    if (device != NULL) {
        // 先停止设备
        device->Stop(device);

        // 销毁底层驱动
        if (device->driver != NULL) {
            L298N_Driver_Destroy(device->driver);
        }

        free(device);
    }
}

// ============================================================================
// 测试函数
// ============================================================================

/**
 * @brief 传送带设备测试
 * @return 成功返回0
 */
int Conveyor_Device_Test(void)
{
    // 创建传送带设备实例 (使用预定义配置)
    P_Conveyor_Device conveyor = Conveyor_Device_Create();

    if (conveyor == NULL) return -1;

    // 初始化
    conveyor->Init(conveyor);

    // 显示设备信息
    const char *name = conveyor->GetName(conveyor);

    // 测试前进
    conveyor->Forward(conveyor, CONVEYOR_DEFAULT_SPEED);
    delay_ms(2000);

    // 测试停止
    conveyor->Stop(conveyor);
    HAL_Delay(CONVEYOR_SAFETY_DELAY_MS);

    // 测试后退
    conveyor->Backward(conveyor, CONVEYOR_DEFAULT_SPEED);
    delay_ms(2000);

    // 最终停止
    conveyor->Stop(conveyor);

    // 获取状态
    int direction = conveyor->GetDirection(conveyor);
    int speed = conveyor->GetSpeed(conveyor);

    // 清理资源
    Conveyor_Device_Destroy(conveyor);
    return 0;
}
