/**
@file K230_uart_dma.h
@brief K230 串口 DMA 相关的数据结构与接口声明（用户注释版）
@author sleet
@date 2025/9/30
*/

#ifndef K230_UART_DMA_H
#define K230_UART_DMA_H

#include "usart.h"
#include "main.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    /** 类别名称数组（只读） */
    extern const char* class_names[];

#define CLASS_COUNT 3  // 类别数量

    /** 检测结果数据结构 */
    typedef struct {
        int x;          /**< 左上角 X */
        int y;          /**< 左上角 Y */
        int width;      /**< bbox 宽度 */
        int height;     /**< bbox 高度 */
        int class_id;   /**< 类别 ID */
        const char* class_name; /**< 类别名称字符串 */
        float confidence; /**< 置信度（0..1） */
    } DetectionResult;

    /** K230 总体数据结构，包含多个检测 */
    typedef struct {
        int total_count;                    /**< 检测总数 */
        DetectionResult detections[10];     /**< 原始检测数组，最多 10 个 */
        DetectionResult latest_detections[3]; /**< 每个类别的最新检测（0:apple,1:strawberry,2:watermelon） */
    } K230Data;

    /** 外部变量声明 */
    extern uint8_t k230_usar_data[200];
    extern K230Data k230;
    extern uint8_t K230_Apple_flag, K230_Strawberry_flag, K230_Watermelon_flag;

    /**
     * @brief 初始化 K230 UART 的 DMA（启动空闲接收）
     */
    void K230_urat_dma_Init(void);

    /**
     * @brief 解析 K230 发来的分号分隔字符串并更新内部数据结构
     * @param data_str 输入字符串指针（函数将使用 strtok 修改该缓冲区）
     */
    void ParseK230Data(char *data_str);

    /**
     * @brief 根据类别 id 返回对应的名称
     * @param class_id 类别 id
     * @return 对应名称字符串（越界返回 "unknown"）
     */
    const char* get_class_name(int class_id);

#ifdef __cplusplus
}
#endif

#endif // K230_UART_DMA_H
