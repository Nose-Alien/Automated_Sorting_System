"""
实验名称：在线训练-YOLO图像检测: 基于摄像头
实验平台：01Studio CanMV K230/CanMV K230 mini
说明：可以通过display="xxx"参数选择"hdmi"、"lcd3_5"(3.5寸mipi屏)或"lcd2_4"(2.4寸mipi屏)显示方式
01科技（01Studio）在线训练平台：https://ai.01studio.cc
"""

from libs.PipeLine import PipeLine
from libs.YOLO import YOLO11
from libs.Utils import *
from media.sensor import *
import os, sys, gc
import ulab.numpy as np
import image
import time

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
confidence_threshold = 0.6  # 置信度
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
    print(res)  # 打印识别结果
    pl.show_image()
    gc.collect()

    print("FPS:", clock.fps())  # 打印帧率

# 释放资源
yolo.deinit()
pl.destroy()
