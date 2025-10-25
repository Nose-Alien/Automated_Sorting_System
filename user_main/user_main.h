/**
@file user_main.h
@brief 用户主程序头文件与接口声明
@author sleet
@date 2025/9/30
*/

#ifndef USER_MAIN_H
#define USER_MAIN_H

#ifdef __cplusplus
extern "C" {

#endif
#include "main.h"

/** 应用级计数变量（在 user_main.c 中定义） */
extern  uint32_t Apple_Count,Strawberry_Count,Watermelon_Count, Sum;

/**
  * @brief 启动用户应用的入口（在 main 之外被调用）
  * @return int 通常不返回
  */
int user_main();

/**
  * @brief 初始化所有用户模块与外设（传感器、通信、机械臂等）
  * @note 请在该函数中避免长时间阻塞操作
  */
void user_main_int();

/**
  * @brief 执行机械臂抓取放置的动作序列
  */
void bot_arm_Action_group(void);

/**
  * @brief 将内部状态写入 DWIN 屏（数值/开关/角度）
  */
void DWIN_update(void);

/**
  * @brief 主循环逻辑入口，负责协调各子模块
  */
void run();

#ifdef __cplusplus
}
#endif

#endif //USER_MAIN_H
