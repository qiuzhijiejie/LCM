# This program is for learning only,Not to be used for any other purpose 
# without the permission of the author
# Testing Hardware:Raspberry PI 3 Model B and B+,  basic frequency:1GHz
# QDtech-OLED liquid crystal driver for Raspberry PI
# xiaofeng@ShenZhen QDtech co.,LTD
# Company Website:www.qdtft.com
# Taobao Website:http://qdtech.taobao.com
# wiki Technology Website:http://www.lcdwiki.com
# We provide technical support,Any technical questions are welcome to 
# exchange and study at any time
# Fixed telephone (fax):+86 0755-23594567 
# cell-phone:15989313508(Mr Feng)
# E-mail:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
# Technical Support QQ:3002773612  3002778157
# Technical Exchange QQ group:324828016
# Date:2018/9/14
# Version:V1.0
# Copyright reserved, piracy must be investigated
# Copyright(C) ShenZhen QDtech co.,LTD 2018-2028
# All rights reserved
#****************************************************************************************************
# This program uses the bcm2835 gpio library,
# so the pin definition using BCM coding
#=====================================power supply wiring===========================================//
# OLED Module                Raspberry PI    
#    VCC        connect       DC 5V/3.3V      //OLED power positive, Physical pin 1,2,4
#    GND        connect          GND          //OLED power ground,Physical pin 6,9,14,20,25,30,34,39
#======================================data line wiring=============================================//
# The default data bus type for this module is iic 
#  OLED Module                Raspberry PI 
#    SDA        connect         3(bcm:2)      //OLED iic write signal
#======================================control line wiring==========================================//
# OLED Module                 Raspberry PI 
#    SCL        connect         5(bcm:3)      //OLED iic colck signal
#========================================touch screen wiring========================================//
#This module has no touch function,so it don't need touch screen wiring
#*****************************************************************************************************/	
#****************************************************************************************************
# @attention
#
# THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
# WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
# TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
# DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
# FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
# CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
#*****************************************************************************************************/

import smbus2 as IIC
import oled
import time
import random
import datetime
import os

import Image
import ImageDraw
import ImageFont

#get bmp path
curpath = os.getcwd()
bmp1 = curpath+"/../bmp/QDlogo.bmp"
bmp2 = curpath+"/../bmp/phonecall.bmp"
bmp3 = curpath+"/../bmp/running.bmp"
bmp4 = curpath+"/../bmp/weather.bmp"
bmp5 = curpath+"/../bmp/timeUI.bmp"
font1 = curpath+"/../font/english_font.ttf"

# OLED module display with hardware SPI:
myoled = oled.SH1106(IIC.SMBus(1))

# Initialize oled
myoled.oledinit()

# clear oled module
myoled.oledclear(0)
myoled.oleddisplay()


# get blank image for drawing.
Width = myoled.width
Height = myoled.height

# show logo
oledbmp = Image.open(bmp1).convert('1')
display = ImageDraw.Draw(oledbmp)
myoled.oledimage(oledbmp)
myoled.oleddisplay()
time.sleep(0.5)
myoled.oledinvertdisplay(0)

time.sleep(1)

# show call phone
oledbmp = Image.open(bmp2).convert('1')
display = ImageDraw.Draw(oledbmp)
myoled.oledimage(oledbmp)
myoled.oleddisplay()

time.sleep(1)
# show running
oledbmp = Image.open(bmp3).convert('1')
display = ImageDraw.Draw(oledbmp)
myoled.oledimage(oledbmp)
myoled.oleddisplay()

time.sleep(1)

# show menu

oledbmp = Image.open(bmp4).convert('1')
display = ImageDraw.Draw(oledbmp)
display.line((0,11,Width-1,11),fill=255)
display.line((Width/2-1,12,Width/2-1,Height-1),fill=255)
display.line((Width/2-1,11+(Height-10)/2,Width-1,11+(Height-10)/2),fill=255)
oledfont = ImageFont.load_default()
display.text((0,0),"2018-08-25",font=oledfont,fill=255)
display.text((78,0),"Saturday",font=oledfont,fill=255)
display.text((14,Height-1-10),"Cloudy",font=oledfont,fill=255)
display.text((Width/2-1+2,12),"TEMP",font=oledfont,fill=255)
display.ellipse((Width-1-18,28,Width-1-15,31),outline=255)
display.text((Width-1-14,28),"C",font=oledfont,fill=255)
display.text((Width/2-1+2,39),"PM2.5",font=oledfont,fill=255)
display.text((Width/2-1+31,53),"ug/m3",font=oledfont,fill=255)
oledfont = ImageFont.truetype(font1,20)
display.text((Width/2-1+14,21),"32",font=oledfont,fill=255)
display.text((Width/2-1+5,48),"90",font=oledfont,fill=255)
myoled.oledimage(oledbmp)
myoled.oleddisplay()
for i in range(5):
        temp = random.randint(0,40)
        pm = random.randint(0,99)       
        display.rectangle((Width/2-1+14,21,Width/2-1+44,36),fill=0)
        display.rectangle((Width/2-1+5,48,Width/2-1+37,62),fill=0)
        display.text((Width/2-1+14,21),str(temp),font=oledfont,fill=255)
        display.text((Width/2-1+5,48),str(pm),font=oledfont,fill=255)
        myoled.oledimage(oledbmp)
        myoled.oleddisplay()
        time.sleep(0.5)

# show time
oledbmp = Image.open(bmp5).convert('1')
display = ImageDraw.Draw(oledbmp)
timefont = ImageFont.truetype(font1,32)
datefont = ImageFont.truetype(font1,16)
while True:
        now = datetime.datetime.now()
        todaydate = now.strftime("%Y-%m-%d")
        todaytime = now.strftime("%H:%M:%S")
        
        display.rectangle((0,11,Width-1,Height-10),fill=0)
        display.text((2,12),todaytime,font=timefont,fill=255)
        display.text((18,37),todaydate,font=datefont,fill=255)

        myoled.oledimage(oledbmp)
        myoled.oleddisplay()
        time.sleep(0.8)

