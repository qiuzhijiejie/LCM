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

import smbus2
import RPi.GPIO as GPIO
import time

OLED_COMMAND = 0
OLED_DATA = 1

IIC_SLAVE_ADDR = 0x3C

COMMAND_MODE = 0x00
DATA_MODE = 0x40

class SH1106(object):
	"""class for SH1106 64*128 1.3inch IIC OLED module."""

	def __init__(self,smbus):
		# set oled display parameter 
		self.width = 128
		self.height = 64
		self.pagesize = 8
		self.ylevel = 0xB0
		self.xlevell = 0x02
		self.xlevelh = 0x10
		self.oledbuffer = [0]*(self.width*self.pagesize)
		GPIO.setmode(GPIO.BCM)
		GPIO.setwarnings(False)
		# Initialize iic
		self.oledsmbus = smbus
	def iic_command(self,val):
		self.oledsmbus.write_byte_data(IIC_SLAVE_ADDR,COMMAND_MODE,val)
	def iic_data(self,val):
		self.oledsmbus.write_byte_data(IIC_SLAVE_ADDR,DATA_MODE,val)
	def writebyte(self,val,flag):
		"""send one byte data to oled module"""
		if flag == OLED_COMMAND:
			self.iic_command(val)
		else:
			self.iic_data(val)
	def oledinit(self):
		self.writebyte(0xAE,OLED_COMMAND) # --display off
		self.writebyte(0x02,OLED_COMMAND) # --set lower column address
		self.writebyte(0x10,OLED_COMMAND) # --set higher column address
		self.writebyte(0x40,OLED_COMMAND) # --Set display start line
		self.writebyte(0xB0,OLED_COMMAND) # --Set page address
		self.writebyte(0x81,OLED_COMMAND) # --contract control
		self.writebyte(0xFF,OLED_COMMAND) # --Set SEG Output Current Brightness
		self.writebyte(0xA1,OLED_COMMAND) # --set segment remap
		self.writebyte(0xA6,OLED_COMMAND) # --normal/reverse
		self.writebyte(0xA8,OLED_COMMAND) # --multiplex ratio
		self.writebyte(0x3F,OLED_COMMAND) # --duty = 1/64
		self.writebyte(0xAD,OLED_COMMAND) # --set charge pump enable
		self.writebyte(0x8B,OLED_COMMAND) # --0x8B  set inner VCC
		self.writebyte(0x32,OLED_COMMAND) # --0X30---0X33  set VPP   8V
		self.writebyte(0xC8,OLED_COMMAND) # --Com scan direction
		self.writebyte(0xD3,OLED_COMMAND) # --set display offset
		self.writebyte(0x00,OLED_COMMAND) # --  0x20
		self.writebyte(0xD5,OLED_COMMAND) # --set osc division
		self.writebyte(0x80,OLED_COMMAND)
		self.writebyte(0xD9,OLED_COMMAND) # --set pre-charge period
		self.writebyte(0x1F,OLED_COMMAND) # --0x22
		self.writebyte(0xDA,OLED_COMMAND) # --set COM pins
		self.writebyte(0x12,OLED_COMMAND)
		self.writebyte(0xDB,OLED_COMMAND) # --set vcomh
		self.writebyte(0x40,OLED_COMMAND)
		self.writebyte(0xAF,OLED_COMMAND) # --display ON
	def oledclear(self,val):
		"""clear oled image buffer"""
		if val == 1:
			self.oledbuffer = [0xff]*(self.width*self.pagesize)
		else:
			self.oledbuffer = [0]*(self.width*self.pagesize)
	def oleddisplay(self):
		"""send oled image buffer data to oled module"""
		for i in range(self.pagesize):
			self.writebyte(self.ylevel+i,OLED_COMMAND)
			self.writebyte(self.xlevell,OLED_COMMAND)
			self.writebyte(self.xlevelh,OLED_COMMAND)
			for n in range(self.width):
				self.writebyte(self.oledbuffer[i*self.width+n],OLED_DATA)
	def oledinvertdisplay(self,val):
		if val == 1:
			self.writebyte(0xA7,OLED_COMMAND)
		else:
			self.writebyte(0xA6,OLED_COMMAND)
	def oledimage(self,image):
		"""set the value of Python Image Library to oled buffer"""
		if image.mode != '1':
			raise ValueError('Image must be in mode 1.')
		imgwidth,imgheight = image.size
		if imgwidth != self.width or imgheight != self.height:
			raise ValueError('Image must be same dimensions as display({0}x{1}).' .format(self.width, self.height))
		pixel = image.load()
		for i in range(self.pagesize):
			for j in range(self.width):
				val = 0
				for k in [0,1,2,3,4,5,6,7]:
					val = val << 1
					val |=0 if pixel[(j,i*8+7-k)] == 0 else 1
				self.oledbuffer[i*self.width+j] = val 
