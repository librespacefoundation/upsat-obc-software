import serial
import array
import time

port = serial.Serial("/dev/tty.usbserial-A50285BI", baudrate=9600, timeout=1.0)

time.sleep(1)

t = array.array('B', [126,24,1,192,185,0,5,16,17,1,6,0,99,126]).tostring() #test service

t = array.array('B', [126,24,1,192,185,0,10,16,8,1,6,1,0,0,0,8,0,124,126]).tostring() #led on

t = array.array('B', [126,24,1,192,185,0,10,16,8,1,6,0,0,0,0,8,0,125,126]).tostring() #led off
port.write(t)

time.sleep(1)

while True:
    rcv = port.read(10)
    print rcv.encode('hex')
    time.sleep(1)