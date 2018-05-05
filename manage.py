import re
import serial
import datetime
from app import app, db
from app.models import Record
from flask_script import Manager


DEVICE = '/dev/ttyUSB0'


#CO2 = 411
#HUM = 31.40
#TEMP = 29.50
manager = Manager(app)

@manager.command
def parse():
    ser = serial.Serial(DEVICE, 9600)
    co2 = None
    hum = None
    temp = None

    co2_ = re.compile('CO2 = (\d+)')
    hum_ = re.compile('HUM = (\d+\.\d+)')
    temp_ = re.compile('TEMP = (\d+\.\d+)')
    while True:
         resp = ser.readline()
         res = resp.decode().strip()

         co2_res = re.match(co2_, res)
         if co2_res:
              co2 = co2_res.group(1)

         hum_res = re.match(hum_, res)
         if hum_res:
              hum = hum_res.group(1)

         temp_res = re.match(temp_, res)
         if temp_res:
              temp = temp_res.group(1)

         if co2 and hum and temp:
              r = Record(co2=co2, hum=hum, temp=temp)
              db.session.add(r)
              db.session.commit()
              co2, hum, temp = None, None, None

if __name__ == "__main__":
    manager.run()


    ser = serial.Serial(DEVICE, 9600)
    while True:
        resp = ser.readline()
        print(datetime.datetime.now(), resp.decode().strip())

