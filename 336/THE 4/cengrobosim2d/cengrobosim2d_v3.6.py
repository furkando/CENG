#!/usr/bin/env python

"""

METU CENG336 SPRING 2019

by Fatih Gokce & Alperen Eroglu based on Mircea Agapie's pyrobosim2d project: 
http://sourceforge.net/projects/pyrobosim2d/

modified by Merve Tapli and Cem Onem 

Simple 2D robot simulator in Python (2.7). You need to have Python, Pygame and PySerial 
installed for it to work. Launch from its own directory
by typing    python cengRoboSim.py   in the console/terminal window. Expects
two image files (background.png, robot.bmp) in the same directory.
Press ESC to exit, 'g' for sending $GO: start message to the PIC and switching to active mode, 
and 't' to toggle the trace visibility.

See the homework file for details.
'''



'''
    Copyright (C) 2011 Mircea Agapie 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.    
"""

DEFAULT_PORT = "/dev/ttyUSB0"
DEFAULT_BAUD = 115200

back_image          = 'backgroundb.png'   #must have this file in same dir.
display_cols        = 800
display_rows        = 500
wall_color          = 'black'
undef_region_color  = 'gray'
gap_color	    = 'blue'

trace_color         = 'black'
trace_arc           = 10        #in degrees, shows on both sides of r.azi
trace_decrease      = -1       #negative, subtracts from robot size to make a smaller trace
trace_width         = 1
leave_trace         = 1         #default mode is not to leave traces

color_of_nothing    = 'white'
sim_version         = 'CENG336 Robosim v1.0'

r_image          = 'robot.bmp'  #must have this file in same dir.
r_edge           = 20
r_init_azi       = 180        #azimuth, in degrees (up is 0)
r_init_x_topleft = 15
r_init_y_topleft = 15
r_init_fwd_speed = 5        #pixels per command
r_init_spin_speed= 9        #degrees per command
r_transparency   = 75       #0 is totally transp., 255 totally opaque
r_count_per_pixel = 1
r_count_per_degree = 1

r_opmode = 0

map_num_pixels = 30
grid_size_x = 16
grid_size_y = 4

blue = "#%02x%02x%02x" % (0, 0, 250)
red = "#%02x%02x%02x" % (250, 0, 0)
black = "#%02x%02x%02x" % (0, 0, 0)

WAITING = 0
GETTING = 1
PARSING = 2

NO_COMMAND = 0
FORWARD = 1
LEFT = 2
RIGHT = 3
STOP = 4
MAP = 5
END = 6

timeout = 120 		#in seconds

#import everything
import os, pygame, sys, threading, serial, time, array,datetime
from pygame.locals import *
import math
import random
from collections import deque

main_dir = os.path.split(os.path.abspath(__file__))[0]
screen = pygame.display.set_mode((display_cols, display_rows))
list_traces = list()


class Miniterm:
    def __init__(self, port, baudrate, parity, rtscts, xonxoff):
	'''We do not recommend to use a USB-to-RS232 converter. However, you may use at your own risk. 
	If you use do not forget to test your code in inek machines before submitting it.
	In this case, replace port inside below initialization with a suitable device, for example '/dev/ttyUSB0'. This device 		name and its number depends on your operating system.'''
        self.serial = serial.Serial(port, baudrate, parity=parity,
                                rtscts=rtscts, xonxoff=xonxoff, timeout=1)

        #self.serial = serial.Serial('/dev/ttyUSB0', baudrate, parity=parity,
        #                        rtscts=rtscts, xonxoff=xonxoff, timeout=1)

	self.state = WAITING
	self.data = ''
	self.command_available = False
	self.command = NO_COMMAND
	self.time = 0
	self.avg_time = -1;
	self.max_time = -999999;
	self.min_time = 999999;
	self.prev_time = -1;
	self.startTime = -1
	self.endTime = -1
	self.remTime = timeout
	self.cmdCount = 0;
	self.commands = deque()

    def start(self):
        self.alive = True
        self.receiver_thread = threading.Thread(target=self.reader)
        self.receiver_thread.setDaemon(1)
        self.receiver_thread.start()

    def stop(self):
        self.alive = False

    def join(self):
        self.receiver_thread.join(0.1)

    def updateFPS(self):
	self.time = time.time()
	#self.time = time.clock()
	diff1 = (self.time-self.startTime)*1000.0
	self.cmdCount = self.cmdCount + 1
	self.avg_time = (diff1)/self.cmdCount
	
	if(self.prev_time != -1):
		diff2 = (self.time-self.prev_time)*1000.0
		print('Current time difference from previous command: '+str(diff2)+' ms')
		if(diff2 < self.min_time):
			self.min_time = diff2
		if(diff2 > self.max_time):
			self.max_time = diff2
		self.prev_time = self.time
	else:
		self.prev_time = self.time

    def reader(self):
	while self.alive:
        	if self.state == WAITING:
			self.data = ''
		        if self.serial.inWaiting() == 0:
		            time.sleep(.01)
		            continue
		        
		        byte = self.serial.read()
			
		        # If a start byte is found, switch state
		        if byte == '$':
		            self.data += byte
			    self.state = GETTING
            	elif self.state == GETTING:
			byte = self.serial.read()

			self.data += byte

			if byte == ':':
				if len(self.data) == 3 or len(self.data) == 5 or len(self.data) == 6:
					if (self.data[1] == 'F'):
						if r_opmode == 1:
							self.updateFPS()
							self.commands.append('F')
						self.command_available = True
					elif (self.data[1] == 'R'):
						if r_opmode == 1:
							self.updateFPS()
							self.commands.append('R')
						self.command_available = True
					elif (self.data[1] == 'L'):
						if r_opmode == 1:
							self.updateFPS()
							self.commands.append('L')
						self.command_available = True
					elif (self.data[1] == 'S'):
						if r_opmode == 1:
							self.updateFPS()
							self.commands.append('S')
						self.command_available = True
					elif (self.data[1] == 'M'):
						#MAP command is handled here, not in mode_active() 
						#self.command = MAP
						#self.command_available = True
						if r_opmode == 1:
							if ord(self.data[4]) == 0:
								color = pygame.Color(gap_color)
								list_map[ord(self.data[2])*16+ord(self.data[3])].color = color
								maparr[ord(self.data[2])][ord(self.data[3])] = ord(self.data[4])
							elif ord(self.data[4]) == 1:
								color = pygame.Color(wall_color)
								list_map[ord(self.data[2])*16+ord(self.data[3])].color = color
								maparr[ord(self.data[2])][ord(self.data[3])] = ord(self.data[4])
							else:
								print('WARNING! Wrong data in Map command. Fifth byte can be 0x01 or 0x00.')
							
					elif (self.data[1] == 'E' and self.data[2] == 'N' and self.data[3] == 'D'):
						#END command
						if r_opmode == 1:
							self.endTime = time.time()
							self.updateFPS()
							self.commands.append('E')		
						self.command_available = True
					else:
						print('WARNING! Invalid Command. Command can be of type F, R, L, S, M or E. However, got ', self.data[1])
				else:
					print('WARNING! Invalid message length! Message was:' + self.data)
				self.state = WAITING



class Trace():
    def __init__(self, from_rect, start_angle, stop_angle):
        self.rect       = from_rect
        self.start_angle= start_angle
        self.stop_angle = stop_angle

class Obstacle(pygame.Rect):
    def __init__(self, x_topleft, y_topleft, width, height, color):
        self.x_topleft  = x_topleft
        self.y_topleft  = y_topleft
        self.width      = width
        self.height     = height
        self.color      = pygame.Color(color)

''' Changes alpha for surfaces with per-pixel alpha; only for small surfaces!
    Sets alpha for WHITE pixels to new_alpha.
    The alpha value is an integer from 0 to 255, 0 is fully transparent and
    255 is fully opaque. '''
def change_alpha_for_white(surface,new_alpha):
    size = surface.get_size()
    if size[0]>300 or size[1]>300:
        return surface
    for y in xrange(size[1]):
	for x in xrange(size[0]):
	    r,g,b,a = surface.get_at((x,y))
	    if r==255 and g==255 and b==255:
                surface.set_at((x,y),(r,g,b,new_alpha))
    return surface

''' Changes alpha for surfaces with per-pixel alpha; only for small surfaces!
    Sets alpha for pixels with alpha == 0 to new_alpha. It is needed b/c
    transform.smoothscale pads image with alpha=0. '''
def change_alpha_for_alpha(surface,new_alpha):
    size = surface.get_size()
    for y in xrange(size[1]):
	for x in xrange(size[0]):
	    r,g,b,a = surface.get_at((x,y))
	    if a<200:
                surface.set_at((x,y),(r,g,b,new_alpha))
    return surface

def draw_traces(target_surf):
    for t in list_traces:
        pygame.draw.arc(target_surf, pygame.Color(trace_color), t.rect,\
                        t.start_angle*math.pi/180, t.stop_angle*math.pi/180, trace_width)


maparr =  [[0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0],
           [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
           [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
           [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1]]

blockpool = [[[0,1,0,0,0,0,0,1,1,1,0,1,0,0,0,0,1],
			[0,1,1,0,1,1,0,0,0,0,0,1,1,0,1,1,1],
			[0,1,0,0,0,1,0,1,1,1,0,1,0,0,0,1,1],
			[0,0,0,1,1,0,0,0,0,1,0,0,0,1,0,1,1],
		[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]],

			[[0,1,0,1,1,1,0,1,1,1,0,1,1,0,0,0,1],
			[0,1,0,0,0,1,0,0,0,0,0,1,1,0,1,1,1],
			[0,0,0,1,0,1,1,0,1,0,1,1,0,0,0,1,1],
			[0,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,1],
		[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]],
		
			[[0,0,1,0,1,1,0,0,1,1,1,1,0,0,0,0,1],
			[1,0,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1],
			[0,0,0,0,1,0,1,1,0,1,0,1,1,1,0,1,1],
			[1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,0,1],
		[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]]]
'''
#no blocks
blocks =  [[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
           [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
           [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
           [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
	   [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]]
'''
random.seed(datetime.datetime.now())
blocks = blockpool[random.randrange(0,3)]
for j in range(17):
	blocks[-1][j] = 1
for i in range(5):
	blocks[i][-1] = 1

blocks[-1][-1] = 1

list_rect_obstacles = []
list_obstacles = []

for i in range(4):
	for j in range(16):
		if blocks[i][j] == 1:
			block = Obstacle(50*j,50*i,50,50, wall_color)
			list_obstacles.append(block)
w = Obstacle(0,200,display_cols,1,wall_color)
list_obstacles.append(w)

for ob in list_obstacles:
    list_rect_obstacles.append(pygame.Rect(ob.x_topleft,ob.y_topleft,ob.width,ob.height))


list_map_rect = []
list_map = []

for i in range(4):
	for j in range(16):
		if maparr[i][j] == -1:
			block = Obstacle((50*j),(50*i+270),50,50, undef_region_color)
			list_map.append(block)
		elif maparr[i][j] == 0:
			block = Obstacle(50*j,50*i+270,50,50, gap_color)
			list_map.append(block)
		elif maparr[i][j] == 1:
			block = Obstacle(50*j,50*i+270,50,50, wall_color)
			list_map.append(block)
w = Obstacle(0,269,display_cols,1,wall_color)
list_map.append(w)
w = Obstacle(0,470,display_cols,1,wall_color)
list_map.append(w)

for ob in list_map:
    list_map_rect.append(pygame.Rect(ob.x_topleft,ob.y_topleft,ob.width,ob.height))

class Robot(pygame.sprite.Sprite):
    def __init__(self, image, x_topleft, y_topleft, azimuth, fwd_speed, spin_speed):

	try:
		self.miniterm = Miniterm(DEFAULT_PORT, DEFAULT_BAUD, 'N',
	    		rtscts=False, xonxoff=False,
	)
	except serial.SerialException, e:
		sys.stderr.write("could not open port %r: %s\n" % (DEFAULT_PORT, e))
		sys.exit(1)

	self.miniterm.start()

        pygame.sprite.Sprite.__init__(self) #call Sprite initializer
        #Sprites must have an image and a rectangle
        self.image          = image
        self.rect           = image.get_rect()
        self.rect.topleft   = x_topleft, y_topleft
        self.fwd_speed      = fwd_speed
        self.spin_speed     = spin_speed
        self.azi            = azimuth       #in degrees
        self.original       = self.image    #unchanging copy, for rotations
        self.opmode         = 0             #0=idle, 1=active
        
	self.spin(0)
	self.xfloat = self.rect.center[0] * 1.0
	self.yfloat = self.rect.center[1] * 1.0

	self.xpixel = self.rect.center[0]
	self.ypixel = self.rect.center[1]

	self.moveSuccesfull = False
       
    def update(self):

        if   (self.opmode == 0): 				#IDLE
			if self.miniterm.command_available == True:
				print('WARNING! Command received in IDLE mode!')
				self.miniterm.command_available = False
			return          				
        elif (self.opmode == 1): self.mode_active()     	#ACTIVE
        elif (self.opmode == 2): 			     			#END
			if self.miniterm.command_available == True:
				print('WARNING! Command received in END mode!')
				self.miniterm.command_available = False
			return          				
        else:
            print 'ERROR! Undefined operation mode!'

    def mode_active(self):	
	if len(self.miniterm.commands) > 0:
		cmd = self.miniterm.commands.popleft()
		#print(cmd)
		if (cmd == 'F'):
			temp_unghi = self.azi*math.pi/180
			walk_dx = -self.fwd_speed*math.sin(temp_unghi)
			walk_dy = -self.fwd_speed*math.cos(temp_unghi)
			self.move(walk_dx, walk_dy)
			self.miniterm.serial.write('$')
			self.miniterm.serial.write('E')
			if self.moveSuccesfull == True:
				self.miniterm.serial.write(self.a2s([self.fwd_speed*r_count_per_pixel]))
			else:
				self.miniterm.serial.write(self.a2s([0x00]))
			self.miniterm.serial.write(':')
		if (cmd == 'R'):
			self.spin(-1.0*self.spin_speed)
			self.miniterm.serial.write('$')
			self.miniterm.serial.write('E')
			self.miniterm.serial.write(self.a2s([self.spin_speed*r_count_per_degree]))
			self.miniterm.serial.write(':')
		if (cmd == 'L'):
			self.spin(self.spin_speed)
			self.miniterm.serial.write('$')
			self.miniterm.serial.write('E')
			self.miniterm.serial.write(self.a2s([self.spin_speed*r_count_per_degree]))
			self.miniterm.serial.write(':')
		if (cmd == 'S'):
			if (self.azi % 90) != 0:
				print('WARNING! You should send STOP command at multiple of 90 degrees.') 
			if ((self.rect.center[0]-25)%50) != 0 or ((self.rect.center[1]-25)%50) != 0:
				print('WARNING! You should send STOP command at center points.')
			self.sense()
		#MAP command is handled serial read, not here.
		#if self.miniterm.command == MAP:
			#updateMap()
			#self.miniterm.command_available = False
		if (cmd == 'E'):
			self.opmode = 2
			r_opmode = 2
			self.miniterm.command_available = False
			self.endWithCommand()
			

		#print ('x: ' + str(self.rect.center[0]) + ' y: ' + str(self.rect.center[1]) + ' theta: ' + str(self.azi))

    def move(self,dx,dy):

	self.xfloat = self.xfloat + dx 
	self.yfloat = self.yfloat + dy	
	
        previous_rect = self.rect           #remember in case undo is necessary

        self.rect = self.rect.move(self.xfloat-self.xpixel,self.yfloat-self.ypixel)
        if self.rect.collidelist(list_rect_obstacles) != -1 or self.rect.topleft[0] < 0 or self.rect.topleft[1] < 0 or self.rect.topleft[0] > display_cols - r_edge or self.rect.topleft[1] > display_rows - r_edge:
            print 'WARNING! I am not able move because of a block in front of me.'
            self.rect = previous_rect                   #undo the move
	    self.xfloat = self.xfloat - dx
	    self.yfloat = self.yfloat - dy
            self.moveSuccesfull = False
        else:                   #if there was no collision
	    self.moveSuccesfull = True
            if leave_trace:     #update trace list
                tr = self.rect.inflate(trace_decrease, trace_decrease)
                list_traces.append(Trace(tr, 90+self.azi-trace_arc, 90+self.azi+trace_arc))

	self.xpixel = self.rect.center[0]
	self.ypixel = self.rect.center[1]
       
    def spin(self,dtheta):
        center = self.rect.center
        self.azi += dtheta
        if self.azi >= 360:        
            self.azi = self.azi-360
        if self.azi < 0:
            self.azi = self.azi+360
        temp_rota_imag = pygame.transform.rotate(self.original, self.azi)
        self.image = pygame.transform.smoothscale(temp_rota_imag,(r_edge,r_edge))
        #smoothscale pads w/pixels having alpha=0. Increasing alpha to see a faint rect.
        self.image = change_alpha_for_alpha(self.image, r_transparency)
        self.rect = self.image.get_rect()
        self.rect.center = center
        '''when using transform.smoothscale, rotations never generate collisions, b/c
        the image always stays a square of edge r_edge'''
        if leave_trace:     #update trace list
            tr = self.rect.inflate(trace_decrease, trace_decrease)
            list_traces.append(Trace(tr, 90+self.azi-trace_arc, 90+self.azi+trace_arc))

    def a2s(self,array):
    	return ''.join(chr(b) for b in array)

    def sendSensorData(self, a,b,c,d):

	self.miniterm.serial.write('$')
	self.miniterm.serial.write('D')	
	self.miniterm.serial.write(self.a2s([blocks[a[0]][a[1]], blocks[b[0]][b[1]], blocks[c[0]][c[1]], blocks[d[0]][d[1]]]))
	self.miniterm.serial.write(':')

    def sense(self):

	i = self.rect.center[1] / 50 # y
	j = self.rect.center[0] / 50 # x

	theta = self.azi

	if theta > 315.0 or theta <= 45:
		self.sendSensorData([i-1,j],[i,j+1],[i+1,j],[i,j-1])
	elif theta > 225 and theta <= 315:
		self.sendSensorData([i,j+1],[i+1,j],[i,j-1],[i-1,j])
	elif theta > 135 and theta <= 225:
		self.sendSensorData([i+1,j],[i,j-1],[i-1,j],[i,j+1])
	elif theta > 45 and theta <= 135:
		self.sendSensorData([i,j-1],[i-1,j],[i,j+1],[i+1,j])
	return

    def endWithCommand(self):
	print('')
	print('You sent END command after '+str(self.miniterm.endTime-self.miniterm.startTime)+' seconds.')
	print('')

	err = 0

	for i in range(4):
		for j in range(16):
			if blocks[i][j] != maparr[i][j]:
				err = 1
				print ('Your map info for ['+str(i)+','+str(j)+'] is wrong!')

	if err == 0:
		print('')
		print('Your map is correct!')
		print('')

	if self.rect.center[0] >=750 and self.rect.center[1] < 50: # inside bonus area
		print('')
		print ('You have successfully moved the robot to target(bonus) area!')
		print('')	
	else:
		print('')
		print ('You could not moved the robot to target(bonus) area!')
		print('')

	print('')
	print('Timing between commands [All three values should be as close as to 50 ms]:')
	print('')
	print('\tAvg: '+str(round(self.miniterm.avg_time,2))+' ms')
	print('\tMax: '+str(round(self.miniterm.max_time,2))+' ms')
	print('\tMin: '+str(round(self.miniterm.min_time,2))+' ms')

	print('')
	print('')
	print('You can reset the simulator by pressing r.')
	print('')

    def endWithTimeout(self):
	print('')
	print('Timeout! Your '+str(timeout)+' seconds are over!')
	print('')

	err = 0	

	for i in range(4):
		for j in range(16):
			if blocks[i][j] != maparr[i][j]:
				err = 1
				print ('Your map info for ['+str(i)+','+str(j)+'] is wrong!')

	if err == 0:
		print('')
		print('Your map is correct!')
		print('')

	if self.rect.center[0] >=750 and self.rect.center[1] < 50: # inside bonus area
		print('')
		print ('You have successfully moved the robot to target(bonus) area!')
		print('')
	else:
		print('')
		print ('You could not moved the robot to target(bonus) area!')
		print('')

	print('')
	print('Timing between commands [All three values should be as close as to 50 ms]:')
	print('')
	print('\tAvg: '+str(round(self.miniterm.avg_time,2))+' ms')
	print('\tMax: '+str(round(self.miniterm.max_time,2))+' ms')
	print('\tMin: '+str(round(self.miniterm.min_time,2))+' ms')

	print('')
	print('')
	print('You can reset the simulator by pressing r.')
	print('')

    def resetEverything(self):
	self.rect.topleft = r_init_x_topleft, r_init_y_topleft
        self.azi            = r_init_azi        
	self.spin(0)
	self.xfloat = self.rect.center[0] * 1.0
	self.yfloat = self.rect.center[1] * 1.0

	self.xpixel = self.rect.center[0]
	self.ypixel = self.rect.center[1]

	self.moveSuccesfull = False
		
	#reset map

	color = pygame.Color(undef_region_color)
	for i in range(4):
		for j in range(16):
			maparr[i][j] = -1
			list_map[i*16+j].color = color

	color = pygame.Color(gap_color)
	maparr[0][0] = 0
	maparr[0][15] = 0
	list_map[0].color = color
	list_map[15].color = color

	self.miniterm.prev_time = 0
	self.miniterm.time = 0
	self.miniterm.prev_time = -1;
	self.miniterm.avg_time = -1;
	self.miniterm.max_time = -999999;
	self.miniterm.min_time = 999999;
	self.miniterm.startTime = -1
	self.miniterm.endTime = -1
	self.miniterm.remTime = timeout
	self.miniterm.cmdCount = 0
	self.miniterm.commands = deque()
	self.miniterm.command_available = False

	return


########end of Robot class########

def load_image(name):
    path = os.path.join(main_dir, name)
    temp_image = pygame.image.load(path).convert_alpha()  #need this if using ppalpha
    return change_alpha_for_white(temp_image, r_transparency)  

###########################################
###########################################
def main():
    global leave_trace, list_traces, r_opmode, maparr, list_map, list_map_rect

    pygame.init()           #also calls display.init()    

    r_sprite = load_image(r_image)
    background  = load_image(back_image)

    #prepare simulation objects
    #clock = pygame.time.Clock()
    r = Robot(r_sprite, r_init_x_topleft, r_init_y_topleft,r_init_azi, r_init_fwd_speed,\
              r_init_spin_speed)
    allsprites = pygame.sprite.RenderPlain((r))

    #display the environment once, right before event loop
    screen.blit(background, (0, 0))
    count = -1
    for ob in list_obstacles:
        count = count + 1
        s = pygame.display.get_surface()
        s.fill(ob.color, list_rect_obstacles[count])
    count = -1
    for ob in list_map:
        count = count + 1
        s = pygame.display.get_surface()
        s.fill(ob.color, list_map_rect[count])

    pygame.display.flip()   
    pygame.display.set_caption(sim_version + ' \tmode: IDLE')

    font = pygame.font.SysFont("calibri",30)
    font2 = pygame.font.SysFont("calibri",18)
    text1 = font.render('ARENA', True,(0,0,0))
    text2 = font.render('CURRENT MAP', True,(0,0,0))
		
    going = True

    while going:
        
        #Event loop################################
        for event in pygame.event.get():
            if event == QUIT:
                going = False
            elif event.type == KEYDOWN:
                if event.key == K_ESCAPE:
		    going = False
                if event.key == K_g:
		    if r.opmode != 2 and r.opmode != 1:
			    r.miniterm.serial.write('$')
			    r.miniterm.serial.write('G')
			    r.miniterm.serial.write('O')
			    r.miniterm.serial.write(':')
		            r.opmode = 1            #ACTIVE mode
			    r_opmode = 1
			    r.miniterm.max_rate = -100
			    r.miniterm.min_rate = 9999
			    r.miniterm.avg_rate = 20
			    r.miniterm.prev_time_valid = 0
			    r.miniterm.cmdCount = 0
			    r.miniterm.command_available = False
			    r.miniterm.state = WAITING
		            pygame.display.set_caption(sim_version + ' \tmode: ACTIVE')
			    r.miniterm.startTime = time.time()
                if event.key == K_r:
		    if r.opmode == 2:
			list_traces = list()
			r.resetEverything()
			pygame.display.set_caption(sim_version + ' \tmode: IDLE')
			r.opmode = 0 # enter IDLE mode
			r_opmode = 0
                if event.key == K_t:        #toggles the tracing mode
                    if leave_trace:
                        leave_trace = 0
                        list_traces = list()
                        print 'Trace is OFF.'
                    else:
                        leave_trace = 1
			print 'Trace is ON.'

        #End of event loop#######################
                        
        #Redrawing    
        allsprites.update()
        screen.blit(background, (0, 0))  #redraws the entire bkgrnd.
        count = -1
        for ob in list_obstacles:
            count = count + 1
            s = pygame.display.get_surface()
	    s.fill(ob.color, list_rect_obstacles[count])
    	count = -1
    	for ob in list_map:
            count = count + 1
            s = pygame.display.get_surface()
            s.fill(ob.color, list_map_rect[count])

	screen.blit(text1,(360.,202.))
	screen.blit(text2,(325.,475.))

	text3 = font2.render('x: '+str(r.rect.center[0])+\
			    ' y: '+str(r.rect.center[1])+\
			    ' Theta: '+str(round(r.azi,0)), True,(0,100,0))

	screen.blit(text3,(20.,230.))

	if(r.opmode == 1):
		r.miniterm.remTime = timeout-(time.time()-r.miniterm.startTime)
		if r.miniterm.remTime <= 0:
			r.opmode = 2
			r_opmode = 2
			pygame.display.set_caption(sim_version + ' \tmode: END')
			r.miniterm.state = WAITING
			r.endWithTimeout()
			text4 = font2.render('Remaining Time: 0.00', True,(0,100,0))
			screen.blit(text4,(220.,230.))
		else:
			text4 = font2.render('Remaining Time: '+str(round(r.miniterm.remTime,2)), True,(0,100,0))
			screen.blit(text4,(220.,230.))

		text5 = font2.render('Timing[ms] Avg: '+str(round(r.miniterm.avg_time,2))+\
						     ' Max: '+str(round(r.miniterm.max_time,2))+\
							 ' Min: '+str(round(r.miniterm.min_time,2)), True,(0,100,0))
		screen.blit(text5,(400.,230.))
	elif(r.opmode == 2):
		text4 = font2.render('Remaining Time: 0.00', True,(0,100,0))
		screen.blit(text4,(220.,230.))
		text5 = font2.render('Timing[ms] Avg: '+str(round(r.miniterm.avg_time,2))+\
						     ' Max: '+str(round(r.miniterm.max_time,2))+\
							 ' Min: '+str(round(r.miniterm.min_time,2)), True,(0,100,0))
		screen.blit(text5,(400.,230.))
    
        draw_traces(screen)
        allsprites.draw(screen)
	
        
        #pygame.display.update()
        pygame.display.flip()   #all changes are drawn at once (double buffer)
        #pygame.time.delay(100)
    pygame.quit()               #also calls display.quit()


if __name__ == '__main__':
	main()

        

