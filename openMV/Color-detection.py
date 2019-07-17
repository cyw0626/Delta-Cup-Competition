import sensor,image,time

sensor.reset()  #初始化摄像头
sensor.set_pixformat(sensor.RGB565)   #设置像素模式为彩色
sensor.set_framesize(sensor.QVGA)   #设置图像大小
sensor.skip_frames(10)    #跳过10帧，使新设置生效
sensor.set_auto_whitebal(False)   #关闭自动白平衡

ROI=(80,30,15,15)   #x=80,y=30处宽为15高为15的取值框

while(True):
  img=sensor.snapshot()   #获取一张图片并返回
  statistics=img.get_statistics(roi=ROI)
  color_l=statistics.l_mode()   #亮度
  color_a=statistics.a_mode()   #红到绿
  color_b=statistics.b_mode()   #黄到蓝
  print(color_l,color_a,color_b)   #输出该区域三个参数
  img.draw_rectangle(ROI)   #用矩形圈出该区域
