import time
from grovepi import *
# Connect the Grove Rotary Angle Sensor to analog port A0
potentiometer = 0
# Connect the LED to digital port D5
led = 5
pinMode(potentiometer,"INPUT")
pinMode(led,"OUTPUT")
time.sleep(1)
while True:
 try:
 # Read sensor value from rotary angle sensor (0 to 1023)
 sensor_value = analogRead(potentiometer)
 # Calculate LED brightess (0 to 255) from sensor value (0 to 1023)
 brightness = int(float(sensor_value)/1023 * 255)
 # Give output to LED
 analogWrite(led,brightness)
 print("sensor_value = %d brightness = %d" %(sensor_value, brightness))

 time.sleep(0.1)

 except KeyboardInterrupt:
 analogWrite(led,0)
 break
 except IOError:
 print ("Error")
