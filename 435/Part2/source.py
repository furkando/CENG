import socket
import time
import sys
import ntplib

#Maximum packet size to send 
MAX_PACKET_SIZE = 1000

class Source():

    """ 
        Constructor for TCP
        Create socket and bind it
    """
    def __init__(self, port=None):
        self.port = port
        try:
            #Open TCP connection to send packets
            self.localSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            if port is not None:
                self.localSocket.bind(('', port))
                self.localSocket.listen()
                print("Socket start at port {}".format(port))
        except socket.error as e:
            print(e)
            sys.exit(-1)

    """  
        Read file with the given chunk size 
    """
    def readFile(self, filename, packet_size=1000):
        with open(filename, 'rb') as file:
            while True:
                packet = file.read(packet_size)
                if not packet:
                    break
                yield packet

    """  
        Send given file to given IP and Port
    """
    def sendFile(self, filename, to):
        print("Start to send file: " + filename)
        #Open new NTPClient
        c = ntplib.NTPClient()     
        response = c.request('time.google.com')
        timer = response.tx_time
        print("First packet send at {}".format(timer))
        try:
            #Connect the Brokers IP with TCP
            self.localSocket.connect(to)
        except:
            print("Cannot connect to {}.".format(to), file=sys.stderr)
            sys.exit(-1)

        # send every single packet to the Broker
        for data in self.readFile(filename, packet_size=(MAX_PACKET_SIZE - 4)):
            self.localSocket.send(data)

        #Change time to String
        startTime = str(timer)
        # Save the starting time to the time.txt
        file = open("time.txt", 'w')
        file.write(startTime)
        file.close()

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
    first one is Destination IP
    second one is Destination Port
    third one is the file we sent
"""
argv = sys.argv
destHost = argv[1]
destPort = int(argv[2])
filename  = argv[3]

# Send the file to given IP and Port via TCP
Source().sendFile(filename, (destHost, destPort))

# Close the connection
Source().shutDown