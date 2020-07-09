import os # Library enables the use of operating system dependent functionality
import time # Library handles time-related tasks
from random import shuffle # Import shuffle() method from the random module
from random import random # Import random() method from the random module
from copy import deepcopy # Enable deep copying
import numpy as np # Import Numpy library
 
# Define constants
ALGORITHM_NAME = "Value_Iteration"
FILENAME = "wow.txt"
THIS_TRACK = "R_track"
START = 'S'
GOAL = 'F'
WALL = '#'
TRACK = '.'
MAX_VELOCITY = 5
MIN_VELOCITY = -5
DISC_RATE = 0.9  # Discount rate, also known as gamma. Determines by how much
                 # we discount the value of a future state s'
ERROR_THRES = 0.001 # Determine when Q-values stabilize (i.e.theta)
PROB_ACCELER_FAILURE = 0.20 # Probability car will try to take action a 
                            # according to policy pi(s) = a and fail.
PROB_ACCELER_SUCCESS = 1 - PROB_ACCELER_FAILURE
NO_TRAINING_ITERATIONS = 10 # A single training iteration runs through all
                            # possible states s
NO_RACES = 10 # How many times the race car does a single time trial from 
              # starting position to the finish line
FRAME_TIME = 0.3 # How many seconds between frames printed to the console
MAX_STEPS = 500 # Maximum number of steps the car can take during time trial
MAX_TRAIN_ITER = 50 # Maximum number of training iterations
 
# Range of the velocity of the race car in both y and x directions
vel_range = range(MIN_VELOCITY, MAX_VELOCITY + 1)
 
# All actions A the race car can take
# (acceleration in y direction, acceleration in x direction)
actions = [(-1,-1), (0,-1), (1,-1),
           (-1,0) , (0,0),  (1,0),
           (-1,1) , (0,1),  (1,1)]
 
def read_environment(filename):
    """
    This method reads in the environment (i.e. racetrack)
    :param str filename
    :return environment: list of lists
    :rtype list
    """
    # Open the file named filename in READ mode.
    # Make the file an object named 'file'
    with open(filename, 'r') as file:
 
        # Read until end of file using readline() 
        # readline() then returns a list of the lines
        # of the input file
        environment_data = file.readlines()
     
    # Close the file
    file.close()
 
    # Initialize an empty list
    environment = []
 
    # Adds a counter to each line in the environment_data list,
    # i is the index of each line and line is the actual contents.
    # enumerate() helps get not only the line of the environment but also 
    # the index of that line (i.e. row)
    for i,line in enumerate(environment_data):
        # Ignore the first line that contains the dimensions of the racetrack
        if i > 0:
            # Remove leading or trailing whitespace if applicable
            line = line.strip()
 
            # If the line is empty, ignore it
            if line == "": continue
 
            # Creates a list of lines. Within each line is a list of 
            # individual characters
            # The stuff inside append(stuff) first creates a new_list = []
            # It then appends all the values in a given line to that new 
            # list (e.g. new_list.append(all values inside the line))
            # Then we append that new list to the environment list.
            # Therefoer, environment is a list of lists.
            environment.append([x for x in line])
 
    # Return the environment (i.e. a list of lists/lines)
    return environment
 
def print_environment(environment, car_position = [0,0]):
    """
    This method reads in the environment and current 
    (y,x) position of the car and prints the environment to the console
    :param list environment
    :param list car_position 
    """
    # Store value of current grid square
    temp = environment[car_position[0]][car_position[1]]
 
    # Move the car to current grid square
    environment[car_position[0]][car_position[1]] = "X"
 
    # Delay 
    time.sleep(FRAME_TIME)
 
    # Clear the printed output
    clear()
 
    # For each line in the environment
    for line in environment: 
 
        # Initialize a string
        text = ""
 
        # Add each character to create a line
        for character in line: 
            text += character
 
        # Print the line of the environment
        print(text)
 
    # Retstore value of current grid square
    environment[car_position[0]][car_position[1]] = temp
 
def clear():
    """
    This method clears the print output
    """   
    os.system( 'clear' )
 
def get_random_start_position(environment):
    """
    This method reads in the environment and selects a random
    starting position on the racetrack (y, x). Note that 
    (0,0) corresponds to the upper left corner of the racetrack.
    :param list environment: list of lines
    :return random starting coordinate (y,x) on the racetrack
    :rtype tuple
    """
    # Collect all possible starting positions on the racetrack
    starting_positions = []
 
    # For each row in the environment
    for y,row in enumerate(environment):
 
        # For each column in each row of the environment
        for x,col in enumerate(row):
 
            # If we are at the starting position
            if col == START:
 
                # Add the coordiante to the list of available
                # starting positions in the environment
                starting_positions += [(y,x)]
 
    # Random shuffle the list of starting positions
    shuffle(starting_positions)
 
    # Select a starting position
    return starting_positions[0]
 
def get_new_velocity(old_vel,accel,min_vel=MIN_VELOCITY,max_vel=MAX_VELOCITY):
    """
    Get the new velocity values
    :param tuple old_vel: (vy, vx)
    :param tuple accel: (ay, ax)
    :param int min_vel: Minimum velocity of the car
    :param int max_vel: Maximum velocity of the car
    :return new velocities in y and x directions
    """
    new_y = old_vel[0] + accel[0] 
    new_x = old_vel[1] + accel[1]
    if new_x < min_vel: new_x = min_vel
    if new_x > max_vel: new_x = max_vel
    if new_y < min_vel: new_y = min_vel
    if new_y > max_vel: new_y = max_vel
     
    # Return the new velocities
    return new_y, new_x
 
def get_new_position(old_loc, vel, environment):
    """
    Get a new position using the old position and the velocity
    :param tuple old_loc: (y,x) position of the car
    :param tuple vel: (vy,vx) velocity of the car
    :param list environment
    :return y+vy, x+vx: (new_y,new_x)
    """
    y,x = old_loc[0], old_loc[1]
    vy, vx = vel[0], vel[1]
 
    # new_y = y+vy, new_x = x + vx    
    return y+vy, x+vx
 
def get_nearest_open_cell(environment, y_crash, x_crash, vy = 0, vx = (
        0), open = [TRACK, START, GOAL]):
    """
    Locate the nearest open cell in order to handle crash scenario.
    Distance is calculated as the Manhattan distance.
    Start from the crash grid square and expand outward from there with
    a radius of 1, 2, 3, etc. Forms a diamond search pattern.
     
    For example, a Manhattan distance of 2 would look like the following:     
            .
           ...
          ..#..
           ... 
            .   
    If velocity is provided, search in opposite direction of velocity so that
    there is no movement over walls
    :param list environment
    :param int ycrash: y coordinate where crash happened
    :param int xcrash: x coordinate where crash happened
    :param int vy: velocity in y direction when crash occurred
    :param int vx: velocity in x direction when crash occurred
    :param list of strings open: Contains environment types
    :return tuple of the nearest open y and x position on the racetrack
    """
    # Record number of rows (lines) and columns in the environment
    rows = len(environment)
    cols = len(environment[0])    
    
    # Add expanded coverage for searching for nearest open cell
    max_radius = max(rows,cols)
 
    # Generate a search radius for each scenario
    for radius in range(max_radius):
 
        # If car is not moving in y direction
        if vy == 0: 
            y_off_range = range(-radius, radius + 1)
        # If the velocity in y-direction is negative
        elif vy < 0:
            # Search in the positive direction
            y_off_range = range(0, radius + 1)
        else:
            # Otherwise search in the negative direction
            y_off_range = range(-radius, 1)
 
        # For each value in the search radius range of y
        for y_offset in y_off_range:
 
            # Start near to crash site and work outwards from there
            y = y_crash + y_offset
            x_radius = radius - abs(y_offset)
 
            # If car is not moving in x direction
            if vx == 0:
                x_range = range(x_crash - x_radius, x_crash + x_radius + 1)
            # If the velocity in x-direction is negative
            elif vx < 0:
                x_range = range(x_crash, x_crash + x_radius + 1)
            # If the velocity in y-direction is positive
            else:
                x_range = range(x_crash - x_radius, x_crash + 1)
 
            # For each value in the search radius range of x
            for x in x_range:
                # We can't go outside the environment(racetrack) boundary
                if y < 0 or y >= rows: continue
                if x < 0 or x >= cols: continue
 
                # If we find and open cell, return that (y,x) open cell
                if environment[y][x] in open: 
                    return(y,x)        
     
    # No open grid squares found on the racetrack
    return
 
def act(old_y, old_x, old_vy, old_vx, accel, environment, deterministic=(
    False),bad_crash = False):
    """
    This method generates the new state s' (position and velocity) from the old
    state s and the action a taken by the race car. It also takes as parameters
    the two different crash scenarios (i.e. go to nearest
    open position on the race track or go back to start)
    :param int old_y: The old y position of the car
    :param int old_x: The old x position of the car
    :param int old_vy: The old y velocity of the car
    :param int old_vx: The old x velocity of the car
    :param tuple accel: (ay,ax) - acceleration in y and x directions
    :param list environment: The racetrack
    :param boolean deterministic: True if we always follow the policy
    :param boolean bad_crash: True if we return to start after crash
    :return s' where s' = new_y, new_x, new_vy, and new_vx
    :rtype int
    """
    # This method is deterministic if the same output is returned given
    # the same input information
    if not deterministic:
 
        # If action fails (car fails to take the prescribed action a)
        if random() > PROB_ACCELER_SUCCESS: 
            #print("Car failed to accelerate!")
            accel = (0,0)
  
    # Using the old velocity values and the new acceleration values,
    # get the new velocity value
    new_vy, new_vx = get_new_velocity((old_vy,old_vx), accel)
 
    # Using the new velocity values, update with the new position
    temp_y, temp_x = get_new_position((old_y,old_x), (new_vy, new_vx),( 
                                     environment))
 
    # Find the nearest open cell on the racetrack to this new position
    new_y, new_x = get_nearest_open_cell(environment, temp_y, temp_x, new_vy,new_vx)
    # If a crash happens (i.e. new position is not equal to the nearest
    # open position on the racetrack
    if new_y != temp_y or new_x != temp_x:
 
        # If this is a crash in which we would have to return to the
        # starting position of the racetrack and we are not yet
        # on the finish line
        if bad_crash and environment[new_y][new_x] != GOAL:
 
            # Return to the start of the race track
            new_y, new_x = get_random_start_position(environment)
         
        # Velocity of the race car is set to 0.
        new_vy, new_vx = 0,0
 
    # Return the new state
    return new_y, new_x, new_vy, new_vx
 
def get_policy_from_Q(cols, rows, vel_range, Q, actions):
    """
    This method returns the policy pi(s) based on the action taken in each state
    that maximizes the value of Q in the table Q[s,a]. This is pi*(s)...the
    best action that the race car should take in each state is the one that 
    maximizes the value of Q. (* means optimal)
    :param int cols: Number of columns in the environment
    :param int rows: Number of rows (i.e. lines) in the environment
    :param list vel_range: Range of the velocity of the race car 
    :param list of tuples actions: actions = [(ay,ax),(ay,ax)....]
    :return pi : the policy
    :rtype: dictionary: key is the state tuple, value is the 
       action tuple (ay,ax)
    """
    # Create an empty dictionary called pi
    pi = {}
 
    # For each state s in the environment
    for y in range(rows): 
        for x in range(cols):
            for vy in vel_range:
                for vx in vel_range:
                    # Store the best action for each state...the one that
                    # maximizes the value of Q.
                    # argmax looks across all actions given a state and 
                    # returns the index ai of the maximum Q value
                    pi[(y,x,vy,vx)] = actions[np.argmax(Q[y][x][vy][vx])]        
                     
    # Return pi(s)
    return(pi)
 
def value_iteration(environment, bad_crash = False, reward = (
        0.0), no_training_iter = NO_TRAINING_ITERATIONS):
    """
    This method is the value iteration algorithm.
    :param list environment
    :param boolean bad_crash
    :param int reward of the terminal states (i.e. finish line)
    :param int no_training_iter
    :return policy pi(s) which maps a given state to an optimal action
    :rtype dictionary
    """
    # Calculate the number of rows and columns of the environment
    rows = len(environment)
    cols = len(environment[0])
 
    # Create a table V(s) that will store the optimal Q-value for each state. 
    # This table will help us determine when we should stop the algorithm and 
    # return the output. Initialize all the values of V(s) to arbitrary values, 
    # except the terminal state (i.e. finish line state) that has a value of 0.
    # values[y][x][vy][vx] 
    # Read from left to right, we create a list of vx values. Then for each
    # vy value we assign the list of vx values. Then for each x value, we assign
    # the list of vy values (which contain a list of vx values), etc.
    # This is called list comprehension.
    values = [[[[random() for _ in vel_range] for _ in vel_range] for _ in (
        line)] for line in environment]
 
    # Set the finish line states to 0
    for y in range(rows):
        for x in range(cols):
            # Terminal state has a value of 0
            if environment[y][x] == GOAL:
                for vy in vel_range:
                    for vx in vel_range:                 
                        values[y][x][vy][vx] = reward
 
    # Initialize all Q(s,a) to arbitrary values, except the terminal state 
    # (i.e. finish line states) that has a value of 0.
    # Q[y][x][vy][vx][ai]
    Q = [[[[[random() for _ in actions] for _ in vel_range] for _ in (
        vel_range)] for _ in line] for line in environment]
 
    # Set finish line state-action pairs to 0
    for y in range(rows):
        for x in range(cols):
            # Terminal state has a value of 0
            if environment[y][x] == GOAL:
                for vy in vel_range:
                    for vx in vel_range:   
                        for ai, a in enumerate(actions):                        
                            Q[y][x][vy][vx][ai] = reward
 
    # This is where we train the agent (i.e. race car). Training entails 
    # optimizing the values in the tables of V(s) and Q(s,a)
    while True:
 
        # Keep track of the old V(s) values so we know if we reach stopping 
        # criterion
        values_prev = deepcopy(values)
 
        # When this value gets below the error threshold, we stop training.
        # This is the maximum change of V(s)
        delta = 0.0
 
        # For all the possible states s in S
        for y in range(rows):
            for x in range(cols):
                for vy in vel_range:
                    for vx in vel_range:
                         
                        # If the car crashes into a wall
                        if environment[y][x] == WALL:
 
                            # Wall states have a negative value
                            # I set some arbitrary negative value since
                            # we want to train the car not to hit walls
                            values[y][x][vy][vx] = -9.9
 
                            # Go back to the beginning
                            # of this inner for loop so that we set
                            # all the other wall states to a negative value
                            continue
 
                        # For each action a in the set of possible actions A
                        for ai, a in enumerate(actions):
 
                            # The reward is -1 for every state except
                            # for the finish line states
                            if environment[y][x] == GOAL:
                                r = reward
                            else:
                                r = -1
 
                            # Get the new state s'. s' is based on the current 
                            # state s and the current action a
                            new_y, new_x, new_vy, new_vx = act(
                                y,x,vy,vx,a,environment,deterministic = True, 
                                bad_crash = bad_crash)
 
                            # V(s'): value of the new state when taking action
                            # a from state s. This is the one step look ahead.
                            value_of_new_state = values_prev[new_y][new_x][
                                new_vy][new_vx]
 
                            # Get the new state s'. s' is based on the current 
                            # state s and the action (0,0)
                            new_y, new_x, new_vy, new_vx = act(
                                y,x,vy,vx,(0,0),environment,deterministic = (
                                    True), bad_crash = bad_crash)
 
                            # V(s'): value of the new state when taking action
                            # (0,0) from state s. This is the value if for some
                            # reason the race car attemps to accelerate but 
                            # fails
                            value_of_new_state_if_action_fails = values_prev[
                                new_y][new_x][new_vy][new_vx]
 
                            # Expected value of the new state s'
                            # Note that each state-action pair has a unique 
                            # value for s'
                            expected_value = (
                                PROB_ACCELER_SUCCESS * value_of_new_state) + (
                                PROB_ACCELER_FAILURE * (
                                    value_of_new_state_if_action_fails))
 
                            # Update the Q-value in Q[s,a]
                            # immediate reward + discounted future value
                            Q[y][x][vy][vx][ai] = r + (
                                DISC_RATE * expected_value)
 
                        # Get the action with the highest Q value
                        argMaxQ = np.argmax(Q[y][x][vy][vx])
 
                        # Update V(s)
                        values[y][x][vy][vx] = Q[y][x][vy][vx][argMaxQ]
 
        # Make sure all the rewards to 0 in the terminal state
        for y in range(rows):
            for x in range(cols):
                # Terminal state has a value of 0
                if environment[y][x] == GOAL:
                    for vy in vel_range:
                        for vx in vel_range:                 
                            values[y][x][vy][vx] = reward
 
        # See if the V(s) values are stabilizing
        # Finds the maximum change of any of the states. Delta is a float.
        delta = max([max([max([max([abs(values[y][x][vy][vx] - values_prev[y][
            x][vy][vx]) for vx in vel_range]) for vy in (
            vel_range)]) for x in range(cols)]) for y in range(rows)])
 
        # If the values of each state are stabilizing, return the policy
        # and exit this method.
        if delta < ERROR_THRES:
            return(get_policy_from_Q(cols, rows, vel_range, Q, actions))
 
    return(get_policy_from_Q(cols, rows, vel_range, Q, actions))
 
def do_time_trial(environment, policy, bad_crash = False, animate = True, 
                  max_steps = MAX_STEPS):
    """
    Race car will do a time trial on the race track according to the policy.   
    :param list environment
    :param dictionary policy: A dictionary containing the best action for a 
        given state. The key is the state y,x,vy,vx and value is the action 
        (ax,ay) acceleration
    :param boolean bad_crash: The crash scenario. If true, race car returns to
        starting position after crashes
    :param boolean animate: If true, show the car on the racetrack at each 
        timestep
    :return i: Total steps to complete race (i.e. from starting position to 
        finish line)
    :rtype int
 
    """
    # Copy the environment
    environment_display = deepcopy(environment)
 
    # Get a starting position on the race track
    starting_pos = get_random_start_position(environment)
    y,x = starting_pos
    vy,vx = 0,0  # We initialize velocity to 0
 
    # Keep track if we get stuck
    stop_clock = 0   
 
    # Begin time trial
    for i in range(max_steps):        
 
        # Show the race car on the racetrack
        if animate: 
            print_environment(environment_display, car_position = [y,x])
         
        # Get the best action given the current state
        a = policy[(y,x,vy,vx)]
 
        # If we are at the finish line, stop the time trial
        if environment[y][x] == GOAL: 
            return i 
         
        # Take action and get new a new state s'
        y,x,vy,vx = act(y, x, vy, vx, a, environment, bad_crash = bad_crash)
 
        # Determine if the car gets stuck
        if vy == 0 and vx == 0:
            stop_clock += 1
        else:
            stop_clock = 0
 
        # We have gotten stuck as the car has not been moving for 5 timesteps
        if stop_clock == 5:
            return max_steps
         
    # Program has timed out
    return max_steps
 
def main():
    """
    The main method of the program
    """   
    print("Welcome to the Racetrack Control Program!")
    print("Powered by the " + ALGORITHM_NAME +
          " Reinforcement Learning Algorithm\n")
    print("Track: " + THIS_TRACK)
    print()
    print("What happens to the car if it crashes into a wall?")
    option_1 = """1. Starts from the nearest position on the track to the 
        place where it crashed."""
    option_2 = """2. Returns back to the original starting position."""
    print(option_1)
    print(option_2)
    crash_scenario = int(input("Crash scenario (1 or 2): "))
    no_training_iter = int(input(
        "Enter the initial number of training iterations (e.g. 5): "))
    print("\nThe race car is training. Please wait...")
 
    # Directory where the racetrack is located
    #racetrack_name = input("Enter the path to your input file: ") 
    racetrack_name = FILENAME
    racetrack = read_environment(racetrack_name)
 
    # How many times the race car will do a single time trial
    races = NO_RACES
 
    while(no_training_iter < MAX_TRAIN_ITER):
     
        # Keep track of the total number of steps
        total_steps = 0
 
        # Record the crash scenario
        bad_crash = False
        if crash_scenario == 1:
            bad_crash = False
        else:
            bad_crash = True
 
        # Retrieve the policy
        policy = value_iteration(racetrack, bad_crash = bad_crash,
                             no_training_iter=no_training_iter) 
  
        for each_race in range(races):
            total_steps += do_time_trial(racetrack, policy, bad_crash = (
            bad_crash), animate = True)
 
        print("Number of Training Iterations: " + str(no_training_iter))
        if crash_scenario == 1:
            print("Bad Crash Scenario: " + option_1)
        else:
            print("Bad Crash Scenario: " + option_2)
        print("Average Number of Steps the Race Car Needs to Take Before " +
              "Finding the Finish Line: " + str(total_steps/races) + " steps\n")
        print("\nThe race car is training. Please wait...")
 
        # Delay 
        time.sleep(FRAME_TIME + 4)
 
        # Testing statistics
        test_stats_file = THIS_TRACK 
        test_stats_file += "_"
        test_stats_file += ALGORITHM_NAME + "_iter"
        test_stats_file += str(no_training_iter)+ "_cr"
        test_stats_file += str(crash_scenario) + "_stats.txt"
 
        ## Open a test_stats_file 
        outfile_ts = open(test_stats_file,"w")
         
        outfile_ts.write(
            "------------------------------------------------------------------\n")
        outfile_ts.write(ALGORITHM_NAME + " Summary Statistics\n")
        outfile_ts.write(
            "------------------------------------------------------------------\n")
        outfile_ts.write("Track: ") 
        outfile_ts.write(THIS_TRACK)
        outfile_ts.write("\nNumber of Training Iterations: " + str(no_training_iter))
        if crash_scenario == 1:
            outfile_ts.write("\nBad Crash Scenario: " + option_1 + "\n")
        else:
            outfile_ts.write("Bad Crash Scenario: " + option_2 + "\n")
        outfile_ts.write("Average Number of Steps the Race Car Took " +
              "Before Finding the Finish Line: " + str(total_steps/races) +
              " steps\n")
 
        # Show functioning of the program
        trace_runs_file = THIS_TRACK 
        trace_runs_file += "_"
        trace_runs_file += ALGORITHM_NAME + "_iter"
        trace_runs_file += str(no_training_iter) + "_cr"
        trace_runs_file += str(crash_scenario) + "_trace.txt"
 
        if no_training_iter <= 5:
            ## Open a new file to save trace runs
            outfile_tr = open(trace_runs_file,"w") 
         
            # Print trace runs that demonstrate proper functioning of the code
            outfile_tr.write(str(policy))  
             
            outfile_tr.close()
 
        ## Close the files        
        outfile_ts.close()
 
        no_training_iter += 5
 
main()
