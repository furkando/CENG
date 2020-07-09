import socket
import time

UDP_IP = "10.10.5.2"
UDP_PORT = 12003
UDP_PORT2 = 12005

#Create socket to get packets from broker with UDP
sock = socket.socket(socket.AF_INET, 
                     socket.SOCK_DGRAM) 

#Create socket to send packets from router to destination with UDP
sock2 = socket.socket(socket.AF_INET, 
                        socket.SOCK_DGRAM)              
sock.bind(('', UDP_PORT))

#Get 100 packets from broker end sent it to the destination
for i in range(0,10):
    #Sleep to sync with broker              
    time.sleep(1)
    for y in range (0,10):
        #Receive packets from broker
        data, addr = sock.recvfrom(18) 
        print "received message:", data
        #Send packets to destination 
        sock2.sendto(data, (UDP_IP, UDP_PORT2))