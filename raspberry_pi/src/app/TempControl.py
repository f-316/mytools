#!/usr/bin/env python

import RPi.GPIO as GPIO
import lib.i2clcdb as lcd # lcdのライブラリ
from lib.i2clcdb import lcd_show # lcdのライブラリ
import lib.bme280_sample as bme # センサのライブラリ
from datetime import datetime # 日付のライブラリ
from time import sleep	
from time import time	

def logPrint(string):
	with open("dump.txt", "a") as file:
		file.write(string)

def main():

	# 初期化
	lcd.lcd_init()
	GPIO.setmode(GPIO.BCM)
	GPIO.setup(18, GPIO.OUT)
	#GPIO.output(18, False)
	start_time = time()	
	log_endtime = time()
	loop_endtime = time()
	onOff = False
	GPIO.output(18, False)

	LOGITVL = 10
	LOOPITVL = 0.5

	# ファイルを書き込み
	logPrint("Proc Start\n")	
	logPrint("Log Interval {} sec\n".format(LOGITVL))	
	logPrint("Loop Interval {} sec\n".format(LOOPITVL))	

	while 1:
		# センサデータ取得(0:温度, 1:気圧, 2: 湿度)
		dat = [0] * 3
		bme.readDataProc(dat) 
	
		# ログ書き出し
		if (LOGITVL < (time() - log_endtime)):
			now = datetime.now().strftime("%y/%m/%d %H:%M:%S ")	
			interval = "{:2.3f}".format(time() - log_endtime)
			spend = "{:10.3f}".format(time() - start_time)
			log = now + "{:3.1f}".format(dat[0]) + "℃ log_itv:" + interval + " spend:" + spend + "\n"

			# ファイル書き込み
			logPrint(log)
			log_endtime = time()

		# LCD出力(時間がかかる)
		if (dat[0] < 34):
			if(onOff == False):
				GPIO.output(18, True)
				logPrint("ON\n")
				onOff = True
		elif(33 < dat[0]):
			if(onOff == True):
				GPIO.output(18, False)
				logPrint("OFF\n")
				onOff = False

		lcd_show("{:3.1f}".format(dat[0]), 1, 0)
		
		# 遅延処理
		delay = LOOPITVL - (time() - loop_endtime)
		sleep(delay)
		loop_endtime = time()
main()
