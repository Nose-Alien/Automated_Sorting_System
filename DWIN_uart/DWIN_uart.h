//
// Created by nose on 2025/10/24.
//

#ifndef DWIN_UART_H
#define DWIN_UART_H
#include "main.h"

#define DWIN_HEADER_1          0x5A
#define DWIN_HEADER_2          0xA5
#define DWIN_CMD_WRITE         0x82
#define DWIN_CMD_READ          0x83

#define ADDR_APPLE        0x10
#define ADDR_Strawberry        0x11
#define ADDR_Watermelon        0x12
#define ADDR_SYSTEM       0x13
#define ADDR_SUM       0x13
#define ADDR_CLAW         0x20
#define ADDR_NODE0       0x21
#define ADDR_NODE1       0x22
#define ADDR_NODE2       0x23
#define ADDR_NODE3       0x24
#define ADDR_NODE4       0x25
#define ADDR_CONVEYOR     0x26
#define ADDR_SPEED     0x27

#define Write_ON 0x40
#define Write_OFF 0x3F
#define Write_CLAW_ON          0x49
#define Write_CLAW_OFF         0x48
#define ADDR_X 0x10
#define ADDR_Y 0x20
#define ADDR_count 0x30

// 添加解析相关的定义
#define ADDR_READ_CLEAR    0x14
#define ADDR_READ_SETTINGS 0x15
#define ADDR_READ_RETURN   0x28

// 按键值定义
#define KEY_VALUE_ON       0x0040
#define KEY_VALUE_OFF      0x003F
#define KEY_VALUE_CLAW_ON  0x0049
#define KEY_VALUE_CLAW_OFF 0x0048

#define DWIN_UART_time   100

extern uint8_t DWIN_usar_data[200];
extern  uint8_t DWIN_Apple_flag, DWIN_Strawberry_flag, DWIN_Watermelon_flag;
extern  uint8_t DWIN_SYSTEM_flag, DWIN_CLAW_flag, DWIN_CONVEYOR_flag;
extern  uint8_t DWIN__CLEAR_flag, DWIN__SETTINGS_flag, DWIN__RETURN_flag;
extern  uint8_t DWIN_Speed_value;

enum {
    OFF,
    ON
};

// 触控事件枚举
typedef enum {
    DWIN_EVENT_NONE = 0,
    DWIN_EVENT_APPLE_DETECTED_ON,
    DWIN_EVENT_APPLE_DETECTED_OFF,
    DWIN_EVENT_STRAWBERRY_DETECTED_ON,
    DWIN_EVENT_STRAWBERRY_DETECTED_OFF,
    DWIN_EVENT_WATERMELON_DETECTED_ON,
    DWIN_EVENT_WATERMELON_DETECTED_OFF,
    DWIN_EVENT_SYSTEM_ON,
    DWIN_EVENT_SYSTEM_OFF,
    DWIN_EVENT_CLAW_ON,
    DWIN_EVENT_CLAW_OFF,
    DWIN_EVENT_CONVEYOR_ON,
    DWIN_EVENT_CONVEYOR_OFF,
    DWIN_EVENT_CLEAR,
    DWIN_EVENT_SETTINGS,
    DWIN_EVENT_RETURN,
    DWIN_EVENT_NODE0_CHANGED,
    DWIN_EVENT_NODE1_CHANGED,
    DWIN_EVENT_NODE2_CHANGED,
    DWIN_EVENT_NODE3_CHANGED,
    DWIN_EVENT_NODE4_CHANGED,
    DWIN_EVENT_SPEED_CHANGED
} DWIN_Event_t;

// 迪文屏数据结构
typedef struct {
    DWIN_Event_t event;
    uint8_t node_angles[5];
    uint8_t conveyor_speed;
    uint16_t key_value;
} DWIN_Data_t;

void DWIN_urat_dma_Init(void);
void DWIN_urat_dma_test(void);

// 写函数
void DwinWriteAppleState(int state,uint8_t xValue,uint8_t yValue,uint8_t count);
void DwinWriteStrawberryState(int state,uint16_t xValue,uint16_t yValue,uint16_t count);
void DwinWriteWatermelonState(int state,uint16_t xValue,uint16_t yValue,uint16_t count);
void DwinWriteSystemState(int state);
void DwinWriteSum(uint32_t sum); // 改为 uint32_t 避免窄化转换
void DwinWriteClawState(int state);
void DwinWriteConveyorState(int state,uint16_t speed);
void DwinWriteNodeState(int node,uint16_t angle);

// 解析函数
int DWIN_Parse_Data(uint8_t *data, uint16_t size, DWIN_Data_t *dwin_data);
void DWIN_Process_Event(DWIN_Data_t *dwin_data);

#endif //DWIN_UART_H
