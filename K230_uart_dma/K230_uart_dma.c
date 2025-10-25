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
uint8_t k230_usar_data[200];
uint8_t K230_Apple_flag = 0, K230_Strawberry_flag = 0, K230_Watermelon_flag = 0;
K230Data k230;

// 目标区域配置 (可以根据需要调整)
#define TARGET_AREA_X_MIN 300   // 目标区域左边界
#define TARGET_AREA_X_MAX 475   // 目标区域右边界
#define TARGET_AREA_Y_MIN 0     // 目标区域上边界
#define TARGET_AREA_Y_MAX 480   // 目标区域下边界

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
    memset(&k230, 0, sizeof(k230));

    char *token = strtok(data_str, ";");
    int detection_index = 0;
    int current_detection = -1; // 当前正在处理的目标索引

    while (token != NULL && detection_index < 10) {
        // 去除首尾空格
        while (*token == ' ') token++;

        // 根据前缀解析不同类型的数据
        if (strncmp(token, "D:", 2) == 0) {
            // 解析总数量
            k230.total_count = atoi(token + 2);
        } else if (strncmp(token, "B:", 2) == 0) {
            // 开始新的目标检测结果
            current_detection = detection_index;
            if (current_detection < 10) {
                sscanf(token + 2, "%d,%d,%d,%d",
                       &k230.detections[current_detection].x,
                       &k230.detections[current_detection].y,
                       &k230.detections[current_detection].width,
                       &k230.detections[current_detection].height);
            }
        } else if (strncmp(token, "C:", 2) == 0) {
            // 解析类别信息
            if (current_detection >= 0 && current_detection < 10) {
                k230.detections[current_detection].class_id = atoi(token + 2);
                k230.detections[current_detection].class_name = get_class_name(
                    k230.detections[current_detection].class_id);
            }
        } else if (strncmp(token, "S:", 2) == 0) {
            // 解析置信度
            if (current_detection >= 0 && current_detection < 10) {
                k230.detections[current_detection].confidence = atof(token + 2);

                // 将检测结果存储到对应的固定位置
                int class_id = k230.detections[current_detection].class_id;
                if (class_id >= 0 && class_id < 3) {
                    // 覆盖存储到对应的类别位置
                    memcpy(&k230.latest_detections[class_id],
                           &k230.detections[current_detection],
                           sizeof(DetectionResult));
                }

                detection_index++; // 完成一个目标的解析，移动到下一个
                current_detection = -1; // 重置当前目标索引
            }
        }

        token = strtok(NULL, ";");
    }

    // 打印解析结果（调试用）
    printf("=== K230 Detection Results ===\n");
    printf("Total detections: %d\n", k230.total_count);

    for (int i = 0; i < detection_index && i < k230.total_count; i++) {
        printf("Target %d: %s (Confidence: %.2f%%) at (%d, %d) size %dx%d\n",
               i + 1,
               k230.detections[i].class_name,
               k230.detections[i].confidence * 100,
               k230.detections[i].x,
               k230.detections[i].y,
               k230.detections[i].width,
               k230.detections[i].height);
    }
    printf("==============================\n");

    // 添加位置判断逻辑
    for (int i = 0; i < detection_index && i < k230.total_count; i++) {
        int center_x = k230.detections[i].x + k230.detections[i].width / 2;
        int center_y = k230.detections[i].y + k230.detections[i].height / 2;

        printf("Target %d center: (%d, %d)\n", i+1, center_x, center_y);

        // 判断物体是否在目标区域内
        if (center_x > TARGET_AREA_X_MIN && center_x < TARGET_AREA_X_MAX &&
            center_y > TARGET_AREA_Y_MIN && center_y < TARGET_AREA_Y_MAX) {

            printf("Target %d is in target area!\n", i+1);

            switch (k230.detections[i].class_id) {
                case 0: // apple
                    K230_Apple_flag = 1;
                    printf("Apple detected in target area - taking action...\n");
                    break;
                case 1: // strawberry
                    K230_Strawberry_flag = 1;
                    printf("Strawberry detected in target area - taking action...\n");
                    break;
                case 2: // watermelon
                    K230_Watermelon_flag = 1;
                    printf("Watermelon detected in target area - taking action...\n");
                    break;
            }
        } else {
            printf("Target %d is NOT in target area\n", i+1);
        }
    }
}

void K230_urat_dma_Init(void)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, k230_usar_data, sizeof(k230_usar_data));
    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
}