import socket
from thread import *
import threading
import ntplib
import time

#Change state the locked
print_lock = threading.Lock()

def thread(port,routerName):
    UDP_PORT1 = port

	#Create socket to get packets from routers with UDP
    sock = socket.socket(socket.AF_INET, 
                        socket.SOCK_DGRAM)
    sock.bind(('', UDP_PORT1))
	
	#Get 100 packets from each router
    for i in range(0,10):
		#Sleep to sync with routers      
		time.sleep(1)
		for y in range(0,10):
			#Receive packets from router
			data, addr = sock.recvfrom(18)
			#Get current time with NTP
			c = ntplib.NTPClient()
			response = c.request('time.google.com')
			timer = response.tx_time
			#Convert received data to float
			dataFloat = float(data)
			#Get difference of times between current time and time from data to calculate end-to-end delay as an ms
			print "end to end delay for package from ",routerName, ": ",10*i+y+1,"=>",(timer-dataFloat)*1000, "ms"

def Main(): 
	#block until call to release()
    print_lock.acquire() 
    r1 = "router 1"
    r2 = "router 2"
	#Start new threads to het packets from routers simultenaously
    start_new_thread(thread,(12010,r1))
    start_new_thread(thread,(12020,r2))
    while True: 
        pass
  
if __name__ == '__main__': 
    Main() 