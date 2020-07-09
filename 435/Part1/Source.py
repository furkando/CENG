import socket
import time
import ntplib

TCP_IP = "10.10.1.2"
TCP_PORT = 12001

print "TCP target IP:", TCP_IP
print "TCP target port:", TCP_PORT

#Open TCP connection to send packets
sock = socket.socket(socket.AF_INET, 
                     socket.SOCK_STREAM)

#Connect the Brokers IP with TCP
sock.connect((TCP_IP, TCP_PORT))  

#Open new NTPClient
c = ntplib.NTPClient()     

#Send 100 packets with TCP to Broker
for i in range (0,10):
    #Sleep to program because google decline the request after a while
    time.sleep(1)
    for y in range (0,10):
        #Request NTP time
        response = c.request('time.google.com')
        timer = response.tx_time
        print "Timer: =>",(timer)
        #Change time to String
        MESSAGE = repr(timer)
        #Send the time as a packet to broker
        sock.send(MESSAGE)
#Close TCP connection
sock.close()