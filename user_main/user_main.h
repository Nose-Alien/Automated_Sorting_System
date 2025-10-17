
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
