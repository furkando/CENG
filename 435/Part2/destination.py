import socket
import time
import sys
import threading
import ntplib
import os

nextSequnceNumber = 0
i = 0
allPackets = []
output = []
flag = 0

#Maximum packet size to receive 
MAX_PACKET_SIZE = 1000


class Destination:

    """  
        Constructor for RDT over UDP with given port 
        Setup socket to send packets from broker to destination
    """
    def __init__(self, port=None):
        self.port = port
        try:
            #Open UDP connection to receive packets
            self.localSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            if self.port is not None:
                self.localSocket.bind(('', self.port))
                print("Successfully setup socket at {}".format(self.port))
        except socket.error:
            print("Cannot setup the socket.", file=sys.stderr)
            sys.exit(-1)


    """  
        Parse packet and return checkSum, receivedSum, sequence number, flag and data
        Sequence number is 2 bytes
        Checksum is 1 byte
        Flag is 1 byte
        And the rest is our data
    """
    def parsePacket(self, data):
        # calculate new checksum
        checkSum = self.checksum(data[1:])
        receivedSum = int.from_bytes(data[:1], byteorder='little')
        seqnum = int.from_bytes(data[1:3], byteorder='little')
        flag = int.from_bytes(data[3:4], byteorder='little')
        data = data[4:]

        return checkSum, receivedSum, seqnum, flag, data


    """  
        Make packet with the given seqnum,data and flag then calculate checksum and return packet
        Sequence number is 2 byte
        Checksum is 1 byte
        Flag is 1 byte
        And the rest is our data
    """    
    def makePacket(self, seqnum, data, flag=0):    
        packet = (seqnum.to_bytes(2, byteorder='little') + flag.to_bytes(1, byteorder='little') + data)
        # calculate checksum of our packet
        checkSum = self.checksum(packet)
        return checkSum.to_bytes(1, byteorder='little') + packet


    """  
        Calculate checksum with getting sum of our data and bitwise-and it with 0xff to get unsigned value
    """
    def checksum(self, data):
        return sum(data) & 0xff

    """  
        Save all packets 
    """
    def saveAllPackets(self,message):
        global i
        allPackets.append(message)
        i += 1


    """  
        Parse saved packets sort them inorder to sequence number and write them to the output2.txt
    """
    def writeToFile(self):
        global flag
        if flag==0:
            flag = 1
            for i in range (0,len(allPackets)):
                checkSum, sum, seqnum, flag, data = self.parsePacket(allPackets[i])
                output.append([seqnum,data]) 
            # Sort the packets
            output.sort()
            for i in range (0,len(allPackets)):
                allPackets[i] =output[i][1]

            with open("output2.txt", 'wb') as file:
                for data in allPackets:
                    file.write(data)

            print("Saved to output2.txt")
            os._exit(1)

    """  
        Send single packet 
    """
    def recv(self):
        global nextSequnceNumber,allPackets
        ACKS = 0
        NACKS = 0
        # Set timeout to not wait for long
        self.localSocket.settimeout(60) 
        while True:
            try:
                message, address = self.localSocket.recvfrom(MAX_PACKET_SIZE)
                checkSum, sum, seqnum, flag, data = self.parsePacket(message)
                self.saveAllPackets(message)

                # Compare received checkSum and calculated checkSum also check for sequence number is correct
                if checkSum != sum or seqnum != nextSequnceNumber:
                    # Make ACK with next sequnce number
                    ACK = self.makePacket(nextSequnceNumber - 1,bytes())
                    # Increment NACKS because chechksum not equal received sum
                    NACKS += 1
                    # Send ACK to Broker
                    self.localSocket.sendto(ACK, address)
                    print("ACK: {}, NAK: {}.".format(ACKS, NACKS), end='\r')
                else:
                    # Make ACK with next sequnce number
                    ACK = self.makePacket(nextSequnceNumber,bytes())
                    # Send ACK to Broker
                    self.localSocket.sendto(ACK, address)
                    # Increment ACK Number
                    ACKS += 1
                    print("ACKS: {}, NAK: {}.".format(ACKS, NACKS), end='\r')
                    # Change nextSequnce because packet arrived correctly
                    nextSequnceNumber = nextSequnceNumber + 1
                    # Yield the data
                    yield data

                    # Last packet received
                    if flag == 2 or data==b'' :
                        # Connect NTP server to get the current time
                        c = ntplib.NTPClient()     
                        response = c.request('time.google.com')
                        timer = response.tx_time
                        print("Last packet received at {}".format(timer))
                        
                        # Get the time where first packet sent from Source
                        file = open("time.txt", 'r')
                        startTime = float(file.read())
                        file.close()

                        # Calculate the time when all packets arrived
                        print("Total time is : {} seconds".format(timer-startTime))
                        break
            
            except socket.timeout:
                print('Time out')
                break

    """  
        Receive packets from Broker
    """
    def recvPackets(self):
        received = self.recv()
        i = 0
        for data in received:
            i += 1
        # Write all packets to the input2.txt
        self.writeToFile()    

    """  
        Close the connection
    """
    def shutDown(self):
        try:
            self.localSocket.close()
        except:
            pass




""" 
    args we take from command line
    first one is Port for Router 1
    second one is Port for Router 2
"""
argv = sys.argv

port = int(argv[1])
port2 = int(argv[2])

# Construct Sockets to listen given ports
destination = Destination(port)
destination2 = Destination(port2)


print("Waiting For file")


""" 
    Start 2 thread to listen to given ports simultaneously
"""
try:
    # Create thread with given IP and Port
    thread1 = threading.Thread(target=destination.recvPackets)
    thread2 = threading.Thread(target=destination2.recvPackets)
    # Start threads
    thread1.start()
    thread2.start()
    # Join threads to work properly
    thread1.join()
    thread2.join()

except:
    print ("Error: unable to start thread")

# Close the connections
destination.shutDown()
destination2.shutDown()

    

