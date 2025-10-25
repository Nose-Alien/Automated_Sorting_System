/**
* @file user_main.h
  * @brief 用户主程序头文件与接口声明
  * @author Nose_Alien
  * @date 2025/9/30
  */

#ifndef USER_MAIN_H
#define USER_MAIN_H

#ifdef __cplusplus
extern "C" {

#endif
#include "main.h"

extern  uint32_t Apple_Count,Strawberry_Count,Watermelon_Count, Sum;
// extern I2C_HandleTypeDef hi2c1;
// extern TIM_HandleTypeDef htim2;
// extern TIM_HandleTypeDef htim6;

int user_main();
void user_main_int();
void bot_arm_Action_group(void);
void DWIN_update(void);
void run();

#ifdef __cplusplus
}
#endif

#endif //USER_MAIN_H
