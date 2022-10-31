import time
from grovepi import *
# Connect the Grove LED to digital port D4
led = 4
pinMode(led,"OUTPUT")
time.sleep(1)
while True:
 try:
 #Blink the LED
 digitalWrite(led,1) # Send HIGH to switch on LED
 print ("LED ON!")
 time.sleep(5)#ここを変更すると点灯時間が増える
 digitalWrite(led,0) # Send LOW to switch off LED
 print ("LED OFF!")
 time.sleep(1)
 except KeyboardInterrupt: # Turn LED off before stopping
 digitalWrite(led,0)
 break
 except IOError: # Print "Error" if communication error encountered
 print ("Error")
