/**
* @file K230_uart_dma.c
  * @brief K230 串口 DMA 数据解析与回调实现
  * @author Nose_Alien
  * @date 2025/9/30
  */

#include "k230_uart_dma.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// 类别标签映射定义
const char *class_names[] = {
    "apple",
    "strawberry",
    "watermelon"
};

// 全局变量定义
uint8_t data[200];
uint8_t Apple = 0, Strawberry = 0, Watermelon = 0;
K230Data k230_data;

// 根据类别ID获取类别名称
const char *get_class_name(int class_id)
{
    if (class_id >= 0 && class_id < CLASS_COUNT) {
        return class_names[class_id];
    }
    return "unknown";
}

void ParseK230Data(char *data_str)
{
    // 初始化数据结构
    memset(&k230_data, 0, sizeof(k230_data));

    char *token = strtok(data_str, ";");
    int detection_index = 0;
    int current_detection = -1; // 当前正在处理的目标索引

    while (token != NULL && detection_index < 10) {
        // 去除首尾空格
        while (*token == ' ') token++;

        // 根据前缀解析不同类型的数据
        if (strncmp(token, "D:", 2) == 0) {
            // 解析总数量
            k230_data.total_count = atoi(token + 2);
        } else if (strncmp(token, "B:", 2) == 0) {
            // 开始新的目标检测结果
            current_detection = detection_index;
            if (current_detection < 10) {
                sscanf(token + 2, "%d,%d,%d,%d",
                       &k230_data.detections[current_detection].x,
                       &k230_data.detections[current_detection].y,
                       &k230_data.detections[current_detection].width,
                       &k230_data.detections[current_detection].height);
            }
        } else if (strncmp(token, "C:", 2) == 0) {
            // 解析类别信息
            if (current_detection >= 0 && current_detection < 10) {
                k230_data.detections[current_detection].class_id = atoi(token + 2);
                k230_data.detections[current_detection].class_name = get_class_name(
                    k230_data.detections[current_detection].class_id);
            }
        } else if (strncmp(token, "S:", 2) == 0) {
            // 解析置信度
            if (current_detection >= 0 && current_detection < 10) {
                k230_data.detections[current_detection].confidence = atof(token + 2);
                detection_index++; // 完成一个目标的解析，移动到下一个
                current_detection = -1; // 重置当前目标索引
            }
        }

        token = strtok(NULL, ";");
    }

    // 打印解析结果（调试用）
    printf("=== K230 Detection Results ===\n");
    printf("Total detections: %d\n", k230_data.total_count);

    for (int i = 0; i < detection_index && i < k230_data.total_count; i++) {
        printf("Target %d: %s (Confidence: %.2f%%) at (%d, %d) size %dx%d\n",
               i + 1,
               k230_data.detections[i].class_name,
               k230_data.detections[i].confidence * 100,
               k230_data.detections[i].x,
               k230_data.detections[i].y,
               k230_data.detections[i].width,
               k230_data.detections[i].height);
    }
    printf("==============================\n");

    //这里可以添加您的业务逻辑
    for (int i = 0; i < detection_index && i < k230_data.total_count; i++) {
        switch (k230_data.detections[i].class_id) {
            case 0: // apple
                Apple=1;
                printf("Apple detected - taking action...\n");
                break;
            case 1: // strawberry
                Strawberry=1;
                printf("Strawberry detected - taking action...\n");
                break;
            case 2: // watermelon
                Watermelon=1;
                printf("Watermelon detected - taking action...\n");
                break;
        }
    }
}

void K230_urat_dma_Init(void)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, data, sizeof(data));
    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &huart2) {
        // 添加字符串结束符以确保安全解析
        if (Size < sizeof(data)) {
            data[Size] = '\0';
        } else {
            data[sizeof(data) - 1] = '\0';
        }
        ParseK230Data((char *) data);
        // HAL_UART_Transmit_DMA(&huart2, data, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, data, sizeof(data));
        __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
    }
}
