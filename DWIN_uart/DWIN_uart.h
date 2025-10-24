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
#define ADDR_Y 0x20
#define ADDR_count 0x30

extern uint8_t DWIN_usar_data[200];

enum {
    ON,
    OFF
};
void DWIN_urat_dma_Init(void);
void DWIN_urat_dma_test(void);
void DwinWriteAppleState(int state,uint8_t xValue,uint8_t yValue,uint8_t count);
void DwinWriteStrawberryState(int state,uint16_t xValue,uint16_t yValue,uint16_t count);
void DwinWriteWatermelonState(int state,uint16_t xValue,uint16_t yValue,uint16_t count);

#endif //DWIN_UART_H