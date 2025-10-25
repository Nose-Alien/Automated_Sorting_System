//
// Created by nose on 2025/10/24.
//
#include "DWIN_uart.h"

uint8_t DWIN_usar_data[200];
uint8_t DWIN_Apple_flag=0, DWIN_Strawberry_flag=0, DWIN_Watermelon_flag=0;
uint8_t DWIN_SYSTEM_flag=0, DWIN_CLAW_flag=1, DWIN_CONVEYOR_flag=0;
uint8_t DWIN__CLEAR_flag=0, DWIN__SETTINGS_flag=0, DWIN__RETURN_flag=0;
uint8_t DWIN_Speed_value=0;

// 写苹果
void DwinWriteAppleState(int state,uint8_t xValue,uint8_t yValue,uint8_t count)
{
    if (state == ON) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_APPLE, 0x00, 0x00, Write_ON};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),DWIN_UART_time);
    }
    if (state == OFF) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_APPLE, 0x00, 0x00, Write_OFF};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),DWIN_UART_time);
    }
    uint8_t dwin_xValue[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_APPLE, ADDR_X, 0x00, xValue};
    HAL_UART_Transmit(&huart3, dwin_xValue, sizeof(dwin_xValue),DWIN_UART_time);

    uint8_t dwin_yValue[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_APPLE, ADDR_Y, 0x00, yValue};
    HAL_UART_Transmit(&huart3, dwin_yValue, sizeof(dwin_yValue),DWIN_UART_time);

    uint8_t dwin_count[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_APPLE, ADDR_count, 0x00, count};
    HAL_UART_Transmit(&huart3, dwin_count, sizeof(dwin_count),DWIN_UART_time);
}

// 草莓状态控制
void DwinWriteStrawberryState(int state,uint16_t xValue,uint16_t yValue,uint16_t count)
{
    if (state == ON) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Strawberry, 0x00, 0x00, Write_ON};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),DWIN_UART_time);
    }
    if (state == OFF) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Strawberry, 0x00, 0x00, Write_OFF};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),DWIN_UART_time);
    }
    uint8_t dwin_xValue[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Strawberry, ADDR_X, 0x00, xValue};
    HAL_UART_Transmit(&huart3, dwin_xValue, sizeof(dwin_xValue),DWIN_UART_time);

    uint8_t dwin_yValue[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Strawberry, ADDR_Y, 0x00, yValue};
    HAL_UART_Transmit(&huart3, dwin_yValue, sizeof(dwin_yValue),DWIN_UART_time);

    uint8_t dwin_count[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Strawberry, ADDR_count, 0x00, count};
    HAL_UART_Transmit(&huart3, dwin_count, sizeof(dwin_count),DWIN_UART_time);
}
// 西瓜状态控制
void DwinWriteWatermelonState(int state,uint16_t xValue,uint16_t yValue,uint16_t count)
{
    if (state == ON) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Watermelon, 0x00, 0x00, Write_ON};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),DWIN_UART_time);
    }
    if (state == OFF) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Watermelon, 0x00, 0x00, Write_OFF};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),DWIN_UART_time);
    }
    uint8_t dwin_xValue[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Watermelon, ADDR_X, 0x00, xValue};
    HAL_UART_Transmit(&huart3, dwin_xValue, sizeof(dwin_xValue),DWIN_UART_time);

    uint8_t dwin_yValue[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Watermelon, ADDR_Y, 0x00, yValue};
    HAL_UART_Transmit(&huart3, dwin_yValue, sizeof(dwin_yValue),DWIN_UART_time);

    uint8_t dwin_count[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_Watermelon, ADDR_count, 0x00, count};
    HAL_UART_Transmit(&huart3, dwin_count, sizeof(dwin_count),DWIN_UART_time);
}
// 系统状态控制
void DwinWriteSystemState(int state)
{
    if (state == ON) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_SYSTEM, 0x00, 0x00, Write_ON};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),DWIN_UART_time);
    }
    if (state == OFF) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_SYSTEM, 0x00, 0x00, Write_OFF};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),DWIN_UART_time);
    }
}
//  识别写总数
void DwinWriteSum(int sum)
{
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_SUM, 0x10, 0x00, sum};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),DWIN_UART_time);
}

// 机械爪控制
void DwinWriteClawState(int state)
{
    if (state == ON) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_CLAW, 0x00, 0x00, Write_CLAW_ON};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),DWIN_UART_time);
    }
    if (state == OFF) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_CLAW, 0x00, 0x00, Write_CLAW_OFF};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),DWIN_UART_time);
    }
}
// 传送带控制
void DwinWriteConveyorState(int state,uint16_t speed)
{
    if (state == ON) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_CONVEYOR, 0x00, 0x00, Write_ON};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),DWIN_UART_time);
    }
    if (state == OFF) {
        uint8_t dwinCommand[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_CONVEYOR, 0x00, 0x00, Write_OFF};
        HAL_UART_Transmit(&huart3, dwinCommand, sizeof(dwinCommand),DWIN_UART_time);
    }
    uint8_t dwin_speed[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, ADDR_SPEED, 0x00, 0x00, speed};
    HAL_UART_Transmit(&huart3, dwin_speed, sizeof(dwin_speed),DWIN_UART_time);
}
void DwinWriteNodeState(int node,uint16_t angle)
{
    uint8_t dwin_angle[] = {DWIN_HEADER_1, DWIN_HEADER_2, 0x05, DWIN_CMD_WRITE, node, 0x00, 0x00, angle};
    HAL_UART_Transmit(&huart3, dwin_angle, sizeof(dwin_angle),DWIN_UART_time);
}

int DWIN_Parse_Data(uint8_t *data, uint16_t size, DWIN_Data_t *dwin_data)
{
    // 检查帧头和最小长度
    if (size < 7 || data[0] != DWIN_HEADER_1 || data[1] != DWIN_HEADER_2) {
        return 0;
    }

    uint8_t data_len = data[2];
    uint8_t cmd = data[3];

    // 检查数据长度是否匹配
    if (size < (3 + data_len)) {
        return 0;
    }

    // 提取地址
    uint16_t addr = (data[4] << 8) | data[5];

    // 初始化返回数据
    memset(dwin_data, 0, sizeof(DWIN_Data_t));

    switch (cmd) {
        case DWIN_CMD_READ: {
            // 读指令：屏幕触控发送的指令
            if (data_len >= 4) {
                uint8_t read_len = data[6]; // 读取的数据长度

                // 提取键值
                uint16_t key_value = 0;
                if (data_len >= 6) {
                    key_value = (data[7] << 8) | data[8];
                }
                dwin_data->key_value = key_value;

                // 根据地址判断事件类型
                switch (addr) {

                    // 苹果检测
                    case 0x1000:

                        if (key_value == KEY_VALUE_ON) {
                            dwin_data->event = DWIN_EVENT_APPLE_DETECTED_ON;
                        } else if (key_value == KEY_VALUE_OFF) {
                            dwin_data->event = DWIN_EVENT_APPLE_DETECTED_OFF;
                        }
                        break;

                    // 草莓检测
                    case 0x1100:
                        if (key_value == KEY_VALUE_ON) {
                            dwin_data->event = DWIN_EVENT_STRAWBERRY_DETECTED_ON;
                        } else if (key_value == KEY_VALUE_OFF) {
                            dwin_data->event = DWIN_EVENT_STRAWBERRY_DETECTED_OFF;
                        }
                        break;

                    // 西瓜检测
                    case 0x1200:
                        if (key_value == KEY_VALUE_ON) {
                            dwin_data->event = DWIN_EVENT_WATERMELON_DETECTED_ON;
                        } else if (key_value == KEY_VALUE_OFF) {
                            dwin_data->event = DWIN_EVENT_WATERMELON_DETECTED_OFF;
                        }
                        break;

                    // 系统控制
                    case 0x1300:
                        if (key_value == KEY_VALUE_ON) {
                            dwin_data->event = DWIN_EVENT_SYSTEM_ON;
                        } else if (key_value == KEY_VALUE_OFF) {
                            dwin_data->event = DWIN_EVENT_SYSTEM_OFF;
                        }
                        break;

                    // 夹爪控制
                    case 0x2000:
                        if (key_value == KEY_VALUE_CLAW_ON) {
                            dwin_data->event = DWIN_EVENT_CLAW_ON;
                        } else if (key_value == KEY_VALUE_CLAW_OFF) {
                            dwin_data->event = DWIN_EVENT_CLAW_OFF;
                        }
                        break;

                    // 关节控制
                    case 0x2100:
                        dwin_data->event = DWIN_EVENT_NODE0_CHANGED;
                        dwin_data->node_angles[0] = (uint8_t)key_value;
                        break;
                    case 0x2200:
                        dwin_data->event = DWIN_EVENT_NODE1_CHANGED;
                        dwin_data->node_angles[1] = (uint8_t)key_value;
                        break;
                    case 0x2300:
                        dwin_data->event = DWIN_EVENT_NODE2_CHANGED;
                        dwin_data->node_angles[2] = (uint8_t)key_value;
                        break;
                    case 0x2400:
                        dwin_data->event = DWIN_EVENT_NODE3_CHANGED;
                        dwin_data->node_angles[3] = (uint8_t)key_value;
                        break;
                    case 0x2500:
                        dwin_data->event = DWIN_EVENT_NODE4_CHANGED;
                        dwin_data->node_angles[4] = (uint8_t)key_value;
                        break;

                    // 传送带控制
                    case 0x2600:
                        if (key_value == KEY_VALUE_ON) {
                            dwin_data->event = DWIN_EVENT_CONVEYOR_ON;
                        } else if (key_value == KEY_VALUE_OFF) {
                            dwin_data->event = DWIN_EVENT_CONVEYOR_OFF;
                        }
                        break;

                    // 速度控制
                    case 0x2700:
                        dwin_data->event = DWIN_EVENT_SPEED_CHANGED;
                        dwin_data->conveyor_speed = (uint8_t)key_value;
                        break;

                    // 特殊命令
                    case 0x1400:
                        dwin_data->event = DWIN_EVENT_CLEAR;
                        break;
                    case 0x1500:
                        dwin_data->event = DWIN_EVENT_SETTINGS;
                        break;
                    case 0x2800:
                        dwin_data->event = DWIN_EVENT_RETURN;
                        break;
                }
            }
            break;
        }

        default:
            return 0;
    }
    return 1;
}

// 事件处理函数
void DWIN_Process_Event(DWIN_Data_t *dwin_data)
{
    switch (dwin_data->event) {
        case DWIN_EVENT_APPLE_DETECTED_ON:
            printf("DWIN_Apple_flag=1\n");
            DWIN_Apple_flag=1;
            // 设置你的Apple标志
            break;

        case DWIN_EVENT_APPLE_DETECTED_OFF:
            printf("DWIN_Apple_flag=0\n");
            DWIN_Apple_flag=0;
            // 设置你的Apple标志
            break;

        case DWIN_EVENT_STRAWBERRY_DETECTED_ON:
            printf("DWIN_Strawberry_flag=1\n");
            DWIN_Strawberry_flag=1;
            // 处理草莓检测
            break;
        case DWIN_EVENT_STRAWBERRY_DETECTED_OFF:
            printf("DWIN_Strawberry_flag=0\n");
            DWIN_Strawberry_flag=0;
            // 处理草莓检测
            break;

        case DWIN_EVENT_WATERMELON_DETECTED_ON:
            printf("DWIN_Watermelon_flag=1\n");
            DWIN_Watermelon_flag=1;
            // 处理西瓜检测
            break;

        case DWIN_EVENT_WATERMELON_DETECTED_OFF:
            printf("DWIN_Watermelon_flag=0\n");
            DWIN_Watermelon_flag=0;
            // 处理西瓜检测
            break;

        case DWIN_EVENT_SYSTEM_ON:
            printf("System ON\n");
            DWIN_SYSTEM_flag=1;
            // 启动系统
            break;

        case DWIN_EVENT_SYSTEM_OFF:
            printf("System OFF\n");
            DWIN_SYSTEM_flag=0;
            // 停止系统
            break;

        case DWIN_EVENT_CLAW_ON:
            printf("Claw ON\n");

            DWIN_CLAW_flag=1;
            break;

        case DWIN_EVENT_CLAW_OFF:
            printf("Claw OFF\n");
            DWIN_CLAW_flag=0;
            break;

        case DWIN_EVENT_CONVEYOR_ON:
            printf("Conveyor ON\n");
            DWIN_CONVEYOR_flag=1;
            break;

        case DWIN_EVENT_CONVEYOR_OFF:
            printf("Conveyor OFF\n");
            DWIN_CONVEYOR_flag=0;
            break;

        case DWIN_EVENT_SPEED_CHANGED:
            printf("Conveyor speed: %d\n", dwin_data->conveyor_speed);
            DWIN_Speed_value=dwin_data->conveyor_speed;
            break;

        case DWIN_EVENT_NODE0_CHANGED:
        case DWIN_EVENT_NODE1_CHANGED:
        case DWIN_EVENT_NODE2_CHANGED:
        case DWIN_EVENT_NODE3_CHANGED:
        case DWIN_EVENT_NODE4_CHANGED:
            {
                uint8_t node_num = dwin_data->event - DWIN_EVENT_NODE0_CHANGED;
                printf("Node %d angle: %d\n", node_num, dwin_data->node_angles[node_num]);

                // 这里可以添加关节控制逻辑
                // 注意：需要将0-64的范围转换到实际角度
            }
            break;

        case DWIN_EVENT_CLEAR:
            printf("Clear command\n");
            Apple_Count=0;
            Strawberry_Count=0;
            Watermelon_Count=0;
            Sum=0;
            // DWIN__CLEAR_flag=1;
            break;

        case DWIN_EVENT_SETTINGS:
            printf("Settings command\n");
            DWIN__SETTINGS_flag=1;
            break;

        case DWIN_EVENT_RETURN:
            printf("Return command\n");
            DWIN__RETURN_flag=1;

            break;

        default:
            break;
    }
}

void DWIN_urat_dma_Init(void)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, DWIN_usar_data, sizeof(DWIN_usar_data));
    __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
}

void DWIN_urat_dma_test(void)
{
    // 你现有的测试代码
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
