#!/usr/bin/env python

import lib.i2clcdb as lcd # lcdのライブラリ
from lib.i2clcdb import lcd_show # lcdのライブラリ
import lib.bme280_sample as bme # センサのライブラリ
from time import sleep	
from time import time	

def main():
	lcd.lcd_init()
	LOOPITVL = 0.5

	while 1:
		# センサデータ取得(0:温度, 1:気圧, 2: 湿度)
		dat = [0] * 3
		bme.readDataProc(dat) 
		print("{:3.1f}".format(dat[0]))
		lcd_show("{:3.1f}".format(dat[0]), 1, 0)

		# 遅延処理
		sleep(LOOPITVL)
main()
