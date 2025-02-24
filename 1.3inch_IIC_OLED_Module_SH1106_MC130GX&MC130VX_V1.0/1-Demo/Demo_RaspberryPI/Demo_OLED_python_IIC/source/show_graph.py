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

import Image
import ImageDraw
import ImageFont

# OLED module display with hardware IIC:
myoled = oled.SH1106(IIC.SMBus(1))

# Initialize oled
myoled.oledinit()

# clear oled module
myoled.oledclear(0)
myoled.oleddisplay()


# get blank image for drawing.

Width = myoled.width
Height = myoled.height
oledimage = Image.new('1',(Width,Height))

# Get drawing object to draw on image.
display = ImageDraw.Draw(oledimage)

# set image to blank 
display.rectangle((0,0,Width-1,Height-1),outline=0,fill=0)

# lines test
i=0
while i < myoled.width:
        i = i+4
	display.line((0,i/2,Width-1,i/2),fill=255)
	display.line((i,0,i,Height-1),fill=255)
	myoled.oledimage(oledimage)
	myoled.oleddisplay()
time.sleep(0.5)	 
myoled.oledinvertdisplay(1)
time.sleep(0.5)
myoled.oledinvertdisplay(0)

myoled.oledclear(0)
myoled.oleddisplay()

display.rectangle((0,0,Width-1,Height-1),outline=0,fill=0)

# rectangle test
display.rectangle((0,0,Width/2-1,Height-1),fill=255)
display.rectangle((Width/2-1,0,Width/2-1,Height-1),fill=0)
display.rectangle((5,5,Width/2-1-5,Height-1-5),outline=0)
display.rectangle((Width/2-1+5,5,Width-1-5,Height-1-5),outline=255)
myoled.oledimage(oledimage)
myoled.oleddisplay()
time.sleep(0.5)
display.rectangle((5,5,Width/2-1-5,Height-1-5),fill=0)
display.rectangle((Width/2-1+5,5,Width-1-5,Height-1-5),fill=255)
myoled.oledimage(oledimage)
myoled.oleddisplay()
time.sleep(0.5)

display.rectangle((0,0,Width-1,Height-1),outline=0,fill=0)

# circle test
display.rectangle((0,0,Width/2-1,Height-1),fill=255)
display.rectangle((Width/2-1,0,Width/2-1,Height-1),fill=0)
display.ellipse((5,5,Width/2-1-5,Height-1-5),outline=0)
display.ellipse((Width/2-1+5,5,Width-1-5,Height-1-5),outline=255)
myoled.oledimage(oledimage)
myoled.oleddisplay()
time.sleep(0.5)
display.ellipse((5,5,Width/2-1-5,Height-1-5),fill=0)
display.ellipse((Width/2-1+5,5,Width-1-5,Height-1-5),fill=255)
myoled.oledimage(oledimage)
myoled.oleddisplay()
time.sleep(0.5)

display.rectangle((0,0,Width-1,Height-1),outline=0,fill=0)

# ellipse test
display.rectangle((0,0,Width/2-1,Height-1),fill=255)
display.rectangle((Width/2-1,0,Width/2-1,Height-1),fill=0)
display.ellipse((10,5,Width/2-1-10,Height-1-5),outline=0)
display.ellipse((Width/2-1+10,5,Width-1-10,Height-1-5),outline=255)
myoled.oledimage(oledimage)
myoled.oleddisplay()
time.sleep(0.5)
display.ellipse((10,5,Width/2-1-10,Height-1-5),fill=0)
display.ellipse((Width/2-1+10,5,Width-1-10,Height-1-5),fill=255)
myoled.oledimage(oledimage)
myoled.oleddisplay()
time.sleep(0.5)

display.rectangle((0,0,Width-1,Height-1),outline=0,fill=0)

# triangle test
display.rectangle((0,0,Width/2-1,Height-1),fill=255)
display.rectangle((Width/2-1,0,Width/2-1,Height-1),fill=0)
display.polygon([(5,Height-1-5),(Width/2/2-1,5),(Width/2-1-5,Height-1-5)],outline=0)
display.polygon([(Width/2-1+5,Height-1-5),(Width/2-1+Width/2/2,5),(Width-1-5,Height-1-5)],outline=255)
myoled.oledimage(oledimage)
myoled.oleddisplay()
time.sleep(0.5)
display.polygon([(5,Height-1-5),(Width/2/2-1,5),(Width/2-1-5,Height-1-5)],fill=0)
display.polygon([(Width/2-1+5,Height-1-5),(Width/2-1+Width/2/2,5),(Width-1-5,Height-1-5)],fill=255)
myoled.oledimage(oledimage)
myoled.oleddisplay()
time.sleep(0.5)








