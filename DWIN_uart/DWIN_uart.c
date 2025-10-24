//
// Created by nose on 2025/10/24.
//
#include "DWIN_uart.h"

uint8_t DWIN_usar_data[200];


// 写苹果
void DwinWriteAppleState(int state,uint8_t xValue,uint8_t yValue,uint8_t count)
{
    if (state == ON) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_APPLE, 0x00, 0x00, Write_ON};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),100);
    }
    if (state == OFF) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_APPLE, 0x00, 0x00, Write_OFF};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),100);
    }
    uint8_t dwin_xValue[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_APPLE, ADDR_X, 0x00, xValue};
    HAL_UART_Transmit(&huart3, dwin_xValue, sizeof(dwin_xValue),100);

    uint8_t dwin_yValue[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_APPLE, ADDR_Y, 0x00, yValue};
    HAL_UART_Transmit(&huart3, dwin_yValue, sizeof(dwin_yValue),100);

    uint8_t dwin_count[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_APPLE, ADDR_count, 0x00, count};
    HAL_UART_Transmit(&huart3, dwin_count, sizeof(dwin_count),100);
}

// 草莓状态控制
void DwinWriteStrawberryState(int state,uint16_t xValue,uint16_t yValue,uint16_t count)
{
    if (state == ON) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Strawberry, 0x00, 0x00, Write_ON};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),100);
    }
    if (state == OFF) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Strawberry, 0x00, 0x00, Write_OFF};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),100);
    }
    uint8_t dwin_xValue[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Strawberry, ADDR_X, 0x00, xValue};
    HAL_UART_Transmit(&huart3, dwin_xValue, sizeof(dwin_xValue),100);

    uint8_t dwin_yValue[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Strawberry, ADDR_Y, 0x00, yValue};
    HAL_UART_Transmit(&huart3, dwin_yValue, sizeof(dwin_yValue),100);

    uint8_t dwin_count[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Strawberry, ADDR_count, 0x00, count};
    HAL_UART_Transmit(&huart3, dwin_count, sizeof(dwin_count),100);
}
// 西瓜状态控制
void DwinWriteWatermelonState(int state,uint16_t xValue,uint16_t yValue,uint16_t count)
{
    if (state == ON) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Watermelon, 0x00, 0x00, Write_ON};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),100);
    }
    if (state == OFF) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Watermelon, 0x00, 0x00, Write_OFF};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),100);
    }
    uint8_t dwin_xValue[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Watermelon, ADDR_X, 0x00, xValue};
    HAL_UART_Transmit(&huart3, dwin_xValue, sizeof(dwin_xValue),100);

    uint8_t dwin_yValue[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Watermelon, ADDR_Y, 0x00, yValue};
    HAL_UART_Transmit(&huart3, dwin_yValue, sizeof(dwin_yValue),100);

    uint8_t dwin_count[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Watermelon, ADDR_count, 0x00, count};
    HAL_UART_Transmit(&huart3, dwin_count, sizeof(dwin_count),100);
}
// 系统状态控制
void DwinWriteSystemState(int state)
{
    if (state == ON) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_SYSTEM, 0x00, 0x00, Write_ON};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),100);
    }
    if (state == OFF) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_SYSTEM, 0x00, 0x00, Write_OFF};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),100);
    }
}
//  识别写总数
void DwinWriteSum(int sum)
{
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_SUM, 0x10, 0x00, sum};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),100);
}

// 机械爪控制
void DwinWriteClawState(int state)
{
    if (state == ON) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_CLAW, 0x00, 0x00, Write_CLAW_ON};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),100);
    }
    if (state == OFF) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_CLAW, 0x00, 0x00, Write_CLAW_OFF};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),100);
    }
}
// 传送带控制
void DwinWriteConveyorState(int state,uint16_t speed)
{
    if (state == ON) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_CONVEYOR, 0x00, 0x00, Write_ON};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),100);
    }
    if (state == OFF) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_CONVEYOR, 0x00, 0x00, Write_OFF};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),100);
    }
    uint8_t dwin_speed[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_SPEED, 0x00, 0x00, speed};
    HAL_UART_Transmit(&huart3, dwin_speed, sizeof(dwin_speed),100);
}
void DwinWriteNodeState(int node,uint16_t angle)
{
    uint8_t dwin_angle[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, node, 0x00, 0x00, angle};
    HAL_UART_Transmit(&huart3, dwin_angle, sizeof(dwin_angle),100);
}

void DWIN_urat_dma_Init(void)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, DWIN_usar_data, sizeof(DWIN_usar_data));
    __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
}
void DWIN_urat_dma_test(void)
{
    DwinWriteAppleState(ON,100,100,1);
    DwinWriteStrawberryState(ON,100,100,1);
    DwinWriteWatermelonState(ON,100,100,1);
    DwinWriteSystemState(ON);
    DwinWriteSum(100);
    DwinWriteClawState(ON);
    DwinWriteConveyorState(ON,100);
    DwinWriteNodeState(ADDR_NODE0,100);
    delay_ms(500);
    DwinWriteAppleState(OFF,50,50,2);
    DwinWriteStrawberryState(OFF,50,50,2);
    DwinWriteWatermelonState(OFF,50,50,2);
    DwinWriteSystemState(OFF);
    DwinWriteClawState(OFF);
    DwinWriteSum(50);
    DwinWriteConveyorState(OFF,50);
    DwinWriteNodeState(ADDR_NODE0,50);
    delay_ms(500);
}