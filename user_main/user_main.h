
#ifndef USER_MAIN_H
#define USER_MAIN_H

#ifdef __cplusplus
extern "C" {

#endif
#include "main.h"
#include "stm32f1xx_hal.h"

extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;

int user_main();

#ifdef __cplusplus
}
#endif

#endif //USER_MAIN_H
