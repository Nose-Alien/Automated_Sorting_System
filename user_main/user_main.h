/**
* @file user_main.h
  * @brief 用户主程序头文件与接口声明
  * @author sleet
  * @date 2025/9/30
  */

#ifndef USER_MAIN_H
#define USER_MAIN_H

#ifdef __cplusplus
extern "C" {

#endif
#include "main.h"


extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;

int user_main();
void main_int();
void BotArm_ClawPosture(void);

#ifdef __cplusplus
}
#endif

#endif //USER_MAIN_H
