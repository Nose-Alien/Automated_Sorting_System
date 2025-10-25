/**
  * @file user_main.c
  * @brief 用户主程序的入口文件
  * @author Nose_Alien
  * @date 2025/9/30
  */

#include "user_main.h"
#include "k230_uart_dma.h"
#include "conveyor_device.h"
uint32_t TIM4_tick_ms = 0;
uint8_t flag200ms = 0;
uint32_t Apple_Count=0,Strawberry_Count=0,Watermelon_Count=0, Sum = 0;
DWIN_Data_t dwin_data;
P_BotArm arm0;
P_Conveyor_Device conveyor;

// 定义几个常用姿态
float joint_angles_grasp_init[] = {90.0f, 70.0f, 35.0f, 100.0f, 40.0f};     // 夹取初始姿态{90.0f, 70.0f, 35.0f, 100.0f, 40.0f};
float joint_angles_grasp[] = {90.0f, 80.0f, 35.0f, 80.0f, 40.0f};      // 夹取姿态90.0f, 80.0f, 35.0f, 80.0f, 40.0f};
float joint_angles_place_init[] = {150.0f, 70.0f, 35.0f, 100.0f, 40.0f};      // 放置初始姿态{150.0f, 70.0f, 35.0f, 100.0f, 40.0f};
float joint_angles_place[] = {150.0f, 110.0f, 35.0f, 100.0f, 40.0f};      // 放置姿态{150.0f, 110.0f, 35.0f, 100.0f, 40.0f};


/**
  * @brief 主程序入口函数
  * @return int
  */
int user_main()
{
    user_main_int();
    printf("hello world\n");
    while (1) {
        run();
    }
}

void run()
{
    if (DWIN_SYSTEM_flag==1) {
        conveyor->Forward(conveyor, CONVEYOR_DEFAULT_SPEED);

        if (DWIN_Apple_flag==1&&K230_Apple_flag==1) {
            Sum++;
            Apple_Count++;
            K230_Apple_flag = 0;               // 重置标志
            conveyor->Stop(conveyor);
            bot_arm_Action_group();  // 执行机械臂动作
        }
        if (DWIN_Strawberry_flag==1&&K230_Strawberry_flag==1) {
            Sum++;
            Strawberry_Count++;
            K230_Strawberry_flag = 0;               // 重置标志
            conveyor->Stop(conveyor);
            bot_arm_Action_group();  // 执行机械臂动作
        }
        if (DWIN_Watermelon_flag==1&&K230_Watermelon_flag==1) {
            Sum++;
            Watermelon_Count++;
            K230_Watermelon_flag = 0;               // 重置标志
            conveyor->Stop(conveyor);
            bot_arm_Action_group();  // 执行机械臂动作
        }
    }else if (DWIN__SETTINGS_flag==0) {
        conveyor->Stop(conveyor);
    }
    K230_Watermelon_flag = 0;
    K230_Strawberry_flag = 0;
    K230_Apple_flag = 0;

        if (DWIN__SETTINGS_flag==1)
        {
            DWIN_SYSTEM_flag=0;
            DWIN_Apple_flag=0;
            DWIN_Strawberry_flag=0;
            DWIN_Watermelon_flag=0;
            if (DWIN_CLAW_flag==1) {
                arm0->claw_set(arm0, claw_open);
            } else if (DWIN_CLAW_flag==0) {
                arm0->claw_set(arm0, claw_close);
            }

            if (DWIN_CONVEYOR_flag==1) {
                conveyor->Forward(conveyor, DWIN_Speed_value);
            } else if (DWIN_CONVEYOR_flag==0) {
                conveyor->Stop(conveyor);
            }
        uint8_t node_num = dwin_data.event - DWIN_EVENT_NODE0_CHANGED;
        arm0->smooth_set_angle(arm0, node_num, dwin_data.node_angles[node_num]);
        }

        if (DWIN__RETURN_flag==1) {
            conveyor->Stop(conveyor);
            arm0->claw_set(arm0, claw_open);
            arm0->smooth_move_to(arm0, joint_angles_place_init);
            DWIN__SETTINGS_flag=0;
            DWIN__RETURN_flag=0;
            DWIN_CONVEYOR_flag=0;
        }
        if (flag200ms==1) {
            DWIN_update();
            flag200ms=0;
        }
}

void user_main_int()
{
    delay_init(72); //时钟频率
    RetargetInit(&huart1);
    K230_urat_dma_Init();
    DWIN_urat_dma_Init();
    HAL_TIM_Base_Start_IT(&htim4); // 启动定时器并使能中断
    conveyor = Conveyor_Device_Create();
    conveyor->Init(conveyor);
    arm0 = GetBotArmDevice(botarm_0);
    arm0->Init(arm0);
    // 初始化角度记录
    for (int i = 0; i <= 4; i++) {
        arm0->LastAngl[i] = joint_angles_place_init[i];
    }
    for (int i = 0; i <= 4; i++) {
        arm0->set_angle(arm0, i, joint_angles_place_init[i]);
        delay_ms(500);
    }
    arm0->claw_set(arm0, claw_open);
    // arm0->smooth_move_to(arm0, joint_angles_place_init);

}
void bot_arm_Action_group(void)
{

    arm0->smooth_move_to(arm0, joint_angles_grasp_init);
    delay_ms(1000);
    arm0->smooth_move_to(arm0, joint_angles_grasp);
    delay_ms(1000);

    arm0->claw_set(arm0, claw_close);
    delay_ms(1000);
    // 返回到初始姿态
    arm0->smooth_move_to(arm0, joint_angles_grasp_init);
    delay_ms(1000);

    arm0->smooth_move_to(arm0, joint_angles_place);
    delay_ms(1000);

    arm0->claw_set(arm0, claw_open);
    delay_ms(1000);
    // 返回到初始姿态
    arm0->smooth_move_to(arm0, joint_angles_place_init);
    delay_ms(1000);
}

void DWIN_update(void)
{
    // uint8_t apple_x = 0, apple_y = 0;
    // uint8_t strawberry_x = 0, strawberry_y = 0;
    // uint8_t watermelon_x = 0, watermelon_y = 0;

    // 更新苹果状态到迪文屏
    DwinWriteAppleState(DWIN_Apple_flag,k230.latest_detections[0].x ,k230.latest_detections[0].y,Apple_Count);

    // 更新草莓状态到迪文屏
    DwinWriteStrawberryState(DWIN_Strawberry_flag,k230.latest_detections[1].x ,k230.latest_detections[1].y,Strawberry_Count);

    // 更新西瓜状态到迪文屏
    DwinWriteWatermelonState(DWIN_Watermelon_flag,k230.latest_detections[2].x ,k230.latest_detections[2].y,Watermelon_Count);

    // 更新总数到迪文屏
    DwinWriteSum(Sum);

    // // 更新系统状态
    DwinWriteSystemState(DWIN_SYSTEM_flag ? ON : OFF);
    //
    // // 更新夹爪状态
    DwinWriteClawState(DWIN_CLAW_flag ? ON : OFF);
    //
    // // 更新传送带状态和速度
    DwinWriteConveyorState(DWIN_CONVEYOR_flag ? ON : OFF, DWIN_Speed_value);
    //
    DwinWriteNodeState(ADDR_NODE0,(uint16_t)arm0->LastAngl[0]);
    DwinWriteNodeState(ADDR_NODE1,(uint16_t)arm0->LastAngl[1]);
    DwinWriteNodeState(ADDR_NODE2,(uint16_t)arm0->LastAngl[2]);
    DwinWriteNodeState(ADDR_NODE3,(uint16_t)arm0->LastAngl[3]);
    DwinWriteNodeState(ADDR_NODE4,(uint16_t)arm0->LastAngl[4]);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim4) {
        // 判断中断源是否为定时器htim2
        TIM4_tick_ms++; // 每次中断时增加计数
        if (TIM4_tick_ms % 200 == 0) {
            flag200ms=1;
        }
        // 每500毫秒调用一次
        if (TIM4_tick_ms % 500 == 0) {
            HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

        }

        // 每1秒重置计时器计数
        if (TIM4_tick_ms >= 1000) {
            TIM4_tick_ms = 0;
        }
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &huart2) {
        // 添加字符串结束符以确保安全解析
        if (Size < sizeof(k230_usar_data)) {
            k230_usar_data[Size] = '\0';
        } else {
            k230_usar_data[sizeof(k230_usar_data) - 1] = '\0';
        }
        ParseK230Data((char *) k230_usar_data);
        // HAL_UART_Transmit_DMA(&huart2, k230_usar_data, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, k230_usar_data, sizeof(k230_usar_data));
        __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
    }

    if (huart == &huart3) {

        if (DWIN_Parse_Data(DWIN_usar_data, Size, &dwin_data)==1) {
            // 处理迪文屏事件
            DWIN_Process_Event(&dwin_data);
        }

        // HAL_UART_Transmit_DMA(&huart3, DWIN_usar_data, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart3, DWIN_usar_data, sizeof(DWIN_usar_data));
        __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
    }
}
