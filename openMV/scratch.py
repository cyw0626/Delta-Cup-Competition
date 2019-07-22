#库文件引入
import sensor,image,time,pyb
from pyb import UART

red_led = pyb.LED(1)
green_led = pyb.LED(2)
blue_led = pyb.LED(3)

#颜色阈值
wan_threshold=(13,0,9,21,57,-32)     #颜色阈值

bx=by=0

#开启串口通信
uart=UART(3,115200)
uart.write("#1P1000#2P1330#3P1550#4P1485#5P1500#6P1000T3000\r\n")
print("机械臂复位")
time.sleep(5000)

#摄像头检测
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=2000)
clock=time.clock()

#主程序
while(True):
    clock.tick()
    img=sensor.snapshot().lens_corr(1.8)    #畸变矫正
    red_led.on()
    time.sleep(200)
    red_led.off()
    blobs=img.find_blobs([wan_threshold])

    for b in blobs:
        bx=b.cx()
        by=b.cy()
    print("碗坐标：",bx,by)

    if(bx>0 and by>0):
        #开始抓碗
        print("开始抓碗")
        green_led.on()
        time.sleep(200)
        green_led.off()
        uart.write("#6P1000T3000\r\n")
        print("舵机6开始动作，与复位动作相同")
        time.sleep(8000)
        uart.write("#5P1500T3000\r\n")
        print("舵机5开始动作，与复位动作相同")
        time.sleep(8000)
        uart.write("#1P1000T3000\r\n")
        print("舵机1开始动作，与复位动作相同")
        time.sleep(8000)
        uart.write("#3P1300T3000\r\n")
        print("舵机3开始动作")
        time.sleep(8000)
        uart.write("#4P1250T3000\r\n")
        print("舵机4开始动作")
        time.sleep(8000)
        uart.write("#2P1000T3000\r\n")
        print("舵机2开始动作")
        time.sleep(8000)
        uart.write("#6P1872T3000\r\n")
        print("舵机6开始动作")
        time.sleep(8000)
        uart.write("#1P1650T3000\r\n")
        print("舵机1开始动作")
        time.sleep(8000)
        uart.write("#6P1000T3000\r\n")
        print("舵机6开始动作")
        time.sleep(8000)
        #抓碗完成

        print("放碗结束")
        bx=0
        by=0
    else:
        print("碗还没有进入视野")
