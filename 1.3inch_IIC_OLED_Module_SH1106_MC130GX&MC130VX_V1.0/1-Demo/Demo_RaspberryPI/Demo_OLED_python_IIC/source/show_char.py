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

# -*- coding:utf-8 -*-
import smbus2 as IIC
import oled
import time
import os

import Image
import ImageDraw
import ImageFont

# get font path
curpath = os.getcwd()
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
oledimage = Image.new('1',(Width,Height))

# Get drawing object to draw on image.
display = ImageDraw.Draw(oledimage)

# set image to blank 
display.rectangle((0,0,Width-1,Height-1),outline=0,fill=0)

# load default font
oledfont = ImageFont.load_default()

# display main test menu
display.rectangle((0,0,Width-1,Height-1),outline=255,fill=0)
display.rectangle((3,3,Width-1-3,Height-1-3),outline=255,fill=0)
display.text((22, 6), '1.3" oled test', font=oledfont, fill=255)
display.text((46, 16), 'SH1106', font=oledfont, fill=255)
display.text((46, 26), '64X128', font=oledfont, fill=255)
display.text((34, 36), '2018-09-05', font=oledfont, fill=255)
display.text((19, 46), 'www.lcdwiki.com', font=oledfont, fill=255)
myoled.oledimage(oledimage)
myoled.oleddisplay()

time.sleep(1.5)
display.rectangle((0,0,Width-1,Height-1),outline=0,fill=0)

# change font
oledfont = ImageFont.truetype(font1,8)
display.text((5, 6), 'Hello', font=oledfont, fill=255)
oledfont = ImageFont.truetype(font1,16)
display.text((5, 10), 'Hello', font=oledfont, fill=255)
oledfont = ImageFont.truetype(font1,24)
display.text((5, 18), 'Hello', font=oledfont, fill=255)
oledfont = ImageFont.truetype(font1,32)
display.text((5, 30), 'Hello', font=oledfont, fill=255)
myoled.oledimage(oledimage)
myoled.oleddisplay()

time.sleep(1)
display.rectangle((0,0,Width-1,Height-1),outline=0,fill=0)

# show number
oledfont = ImageFont.truetype(font1,8)
display.text((5, 0), '1234567890', font=oledfont, fill=255)
oledfont = ImageFont.truetype(font1,16)
display.text((5, 7), '1234567890', font=oledfont, fill=255)
oledfont = ImageFont.truetype(font1,24)
display.text((5, 19), '123456789', font=oledfont, fill=255)
oledfont = ImageFont.truetype(font1,32)
display.text((5, 36), '1234567', font=oledfont, fill=255)
myoled.oledimage(oledimage)
myoled.oleddisplay()
