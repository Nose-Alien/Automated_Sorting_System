from libs.PipeLine import PipeLine
from libs.YOLO import YOLO11
from libs.Utils import *
from media.sensor import *
import os, sys, gc
import ulab.numpy as np
import image
import time
#导入串口模块
from machine import UART
from machine import FPIOA

fpioa = FPIOA()

# UART2代码
fpioa.set_function(11, FPIOA.UART2_TXD)
fpioa.set_function(12, FPIOA.UART2_RXD)

uart = UART(UART.UART2, 115200) #设置串口号2和波特率
#uart.write('Hello 01Studio!\n') #发送一条数据

# 这里为自动生成内容，自定义场景请修改为您自己的模型路径、标签名称、模型输入大小
kmodel_path="/sdcard/yolo11n_det_640.kmodel"
labels = {0: 'apple', 1: 'strawberry', 2: 'watermelon'}
model_input_size = [640, 640]
# 显示模式，可以选择"hdmi"、"lcd3_5"(3.5寸mipi屏)和"lcd2_4"(2.4寸mipi屏)
display = "lcd3_5"

if display == "hdmi":
    display_mode = "hdmi"
    display_size = [1920, 1080]

elif display == "lcd3_5":
    display_mode = "st7701"
    display_size = [800, 480]

elif display == "lcd2_4":
    display_mode = "st7701"
    display_size = [640, 480]

rgb888p_size = [640, 360]

# 初始化PipeLine
pl = PipeLine(
    rgb888p_size=rgb888p_size, display_size=display_size, display_mode=display_mode
)

if display == "lcd2_4":
    pl.create(sensor=Sensor(width=1280, height=960))  # 创建PipeLine实例，画面4:3
else:
    pl.create(sensor=Sensor(width=1920, height=1080))  # 创建PipeLine实例

display_size = pl.get_display_size()

# 初始化YOLO11实例
confidence_threshold = 0.85  # 置信度
nms_threshold = 0.45
yolo = YOLO11(
    task_type="detect",
    mode="video",
    kmodel_path=kmodel_path,
    labels=labels,
    rgb888p_size=rgb888p_size,
    model_input_size=model_input_size,
    display_size=display_size,
    conf_thresh=confidence_threshold,
    nms_thresh=nms_threshold,
    max_boxes_num=50,
    debug_mode=0,
)
yolo.config_preprocess()

clock = time.clock()

while True:
    clock.tick()

    # 逐帧推理
    img = pl.get_frame()
    res = yolo.run(img)
    yolo.draw_result(res, pl.osd_img)

    # 只在检测到目标时发送数据到串口
    if res and len(res) >= 3 and len(res[0]) > 0:
        # 创建数据字符串
        data_str = ""

        # 添加检测目标数量
        num_detections = len(res[0])
        data_str += f"D:{num_detections};"

        # 添加每个检测目标的原始数据
        for i in range(num_detections):
            # 边界框数据
            bbox = res[0][i]
            # 类别ID
            class_id = res[1][i]
            # 置信度
            score = res[2][i]

            # 将数据添加到字符串
            data_str += f"B:{bbox[0]},{bbox[1]},{bbox[2]},{bbox[3]};"
            data_str += f"C:{class_id};"
            data_str += f"S:{score:.4f};"

        # 发送数据到串口
        try:
            uart.write(data_str + '\n')
            print(f"发送到串口: {data_str}")
        except Exception as e:
            print(f"串口发送错误: {e}")
    else:
        # 没有检测到目标，不发送任何数据
        print("未检测到目标，不发送数据")

    pl.show_image()
    gc.collect()

    print("FPS:", clock.fps())  # 打印帧率

# 释放资源
yolo.deinit()
pl.destroy()
