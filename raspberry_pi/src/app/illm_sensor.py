import TSL2572 as illm_sensor
import i2clcdb as lcd
import time

def main():
    lcd.lcd_init()
    illm_sensor.init()
    LOOPITVL = 1
    lcd.lcd_show("illm_sen Start!", 1, 0)
    time.sleep(3)
    while 1:
        illm = illm_sensor.get_illm()
        # print("{:3.1f}".format(dat[0]))
        # lcd.lcd_show(" " * 16, 1, 0)
        lcd.lcd_show("{:.2f}[lux]".format(illm), 2, 0)
        # 遅延処理
        time.sleep(LOOPITVL)

if __name__ == '__main__':
    main()
    
