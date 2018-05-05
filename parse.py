import serial
import datetime

DEVICE = '/dev/ttyUSB0'

if __name__ == '__main__':
    ser = serial.Serial(DEVICE, 9600)
    while True:
        resp = ser.readline()
        print(datetime.datetime.now(), resp.decode().strip())


