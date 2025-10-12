//
// Created by nose on 2025/10/12.
//

#include "k230_uart_dma.h"

uint8_t data[100];
void K230_urat_dma_Init()
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, data, sizeof(data));
    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart== &huart2) {
        HAL_UART_Transmit_DMA(&huart2, data, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, data, sizeof(data));
        __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
    }
}