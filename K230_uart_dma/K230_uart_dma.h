/**
* @file K230_uart_dma.h
  * @brief K230 串口 DMA 相关数据结构与接口声明
  * @author Nose_Alien
  * @date 2025/9/30
  */

#ifndef __K230_UART_DMA_H__
#define __K230_UART_DMA_H__

#include "usart.h"
#include "main.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    // 先定义结构体，再声明使用它们的变量和函数

    // 类别标签映射声明
    extern const char* class_names[];

#define CLASS_COUNT 3  // 明确指定类别数量，避免 sizeof 计算问题

    // 定义目标检测结果结构体
    typedef struct {
        int x;          // 左上角x坐标
        int y;          // 左上角y坐标
        int width;      // 宽度
        int height;     // 高度
        int class_id;   // 类别ID
        const char* class_name; // 类别名称
        float confidence; // 置信度
    } DetectionResult;

    typedef struct {
        int total_count;                    // 总目标数量
        DetectionResult detections[10];     // 检测结果数组
    } K230Data;

    // 全局变量声明
    extern uint8_t k230_usar_data[200];
    extern K230Data k230;
    extern uint8_t Apple, Strawberry, Watermelon;
    // 函数声明
    void K230_urat_dma_Init(void);
    void ParseK230Data(char *data_str);
    const char* get_class_name(int class_id);

#ifdef __cplusplus
}
#endif

#endif // __K230_UART_DMA_H__