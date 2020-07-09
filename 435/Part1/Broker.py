import socket
import time
import ntplib

TCP_PORT = 12001

UDP_IP = "10.10.3.2" #r1
UDP_IP2 = "10.10.5.2" #r2

UDP_PORT1 = 12010
UDP_PORT2 = 12020 

#Create socket to get packets from source with TCP
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#Create socket to send packets to router1 and router2 with UDP
sock = socket.socket(socket.AF_INET, 
                     socket.SOCK_DGRAM) 

#Listen to TCP port to get packets
s.bind(('', TCP_PORT))
s.listen(1)

conn, addr = s.accept()
print 'Connection address:', addr

#Get 100 packets from source end sent it to the router1 and router2
for i in range(0,10):
    #Sleep to sync with source
    time.sleep(1)
    for y in range (0,10):
        #Receive packets from source
        data = conn.recv(18)
        c = ntplib.NTPClient()
        response = c.request('time.google.com')
        timer = response.tx_time
        #Convert received data to float
        dataFloat = float(data)
        #Get difference of times between current time and time from data to calculate end-to-end delay as an ms
        print "end to end delay for TCP: =>",(timer-dataFloat)*1000, "ms"
        #Send packets to routers 
        sock.sendto(data, (UDP_IP, UDP_PORT1))
        sock.sendto(data, (UDP_IP2, UDP_PORT2))
       

#Close the TCP connection
conn.close()


