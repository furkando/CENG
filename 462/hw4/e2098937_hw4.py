import sys
import random

def getResultVAction(state,state_values):
        next_reward = -1
        action = ""

        for a in actions:
            state_reward = state_values[nextPosition(state,a)]
            if state == nextPosition(state,a):
                state_reward=-1
            if state_reward >= next_reward:
                action = a
                next_reward = state_reward
        return action

def getResultQAction(state,q_values):
        next_reward = -1
        action = ""
        for a in actions:
            current_position = state
            state_reward = q_values[current_position][a]
            if state_reward >= next_reward:
                action = a
                next_reward = state_reward
        return action

def giveReward(state):
    reward = reward_table[state[0]][state[1]]
    return reward

def isEnd(state):
    if (environment[state[0]][state[1]] == 1) or (environment[state[0]][state[1]] == -1):
        return True
    return False


def nextPosition(state, action):
    if action == "N":
        nxtState = (state[0] , state[1]+1)
    elif action == "S":
        nxtState = (state[0] , state[1]-1)
    elif action == "W":
        nxtState = (state[0]-1, state[1] )
    else:
        nxtState = (state[0]+1, state[1] )

    if (nxtState[0] >= 0) and (nxtState[0] < dimx):
        if (nxtState[1] >= 0) and (nxtState[1] < dimy):
            if environment[nxtState[0]][nxtState[1]]!= -2:
                return nxtState
    return state


def chooseQAction(state,q_values):
        next_reward = -1
        action = ""

        if random.uniform(0,1)<= epsilon:
            action = random.choice(actions)
        else:
            for a in actions:
                current_position = state
                state_reward = q_values[current_position][a]
                if state_reward >= next_reward:
                    action = a
                    next_reward = state_reward
        return action

def takeQAction(state, action):
    position = nextPosition(state,action)
    return position

def getRandomState():
    while True:
        x = random.randrange(dimx)
        y = random.randrange(dimx)
        res = environment[x][y]
        # print(x,y)
        if  res!= 1 and res != -1 and res != -2:
            break
    return (x,y)

def qlearning():
    isAgentEnd = False
    isEnvEnd = False
    state = (0,0)
    states=[]
    q_values = {}
    index = 0
    for i in range(dimx):
        for j in range(dimy):
            q_values[(i, j)] = {}
            for a in actions:
                q_values[(i, j)][a] = 0

    while True:
        if isEnvEnd:
            reward=giveReward(state)
            for a in actions:
                q_values[state][a] = reward
            for s in reversed(states):
                current_q_value = q_values[s[0]][s[1]]
                reward = current_q_value + alpha * (gamma * reward - current_q_value)
                q_values[s[0]][s[1]] = reward

            isAgentEnd = False
            isEnvEnd = False
            if(environment[state[0]][state[1]] == 1):
                index+=1
            if(index==episodenum):
                break
            state=getRandomState()
            states=[]
        else:
            action = chooseQAction(state,q_values)
            states.append([(state),action])
            state = takeQAction(state,action)
            isEnvEnd = isEnd(state)
            isAgentEnd = isEnvEnd

    resultString =""
    for i in range(0, dimx):
        for j in range(0, dimy):
            resultString += "{0} {1} {2} \n".format(i+1,j+1,actionmap[getResultQAction((i,j),q_values)])
    with open(sys.argv[2],"w+") as file:
        file.write(resultString)

def chooseVAction(state,state_values):
        next_reward = -1
        action = ""

        if random.uniform(0,1)<= 0.2:
            action = random.choice(actions)
        else:
            for a in actions:
                state_reward = state_values[nextPosition(state,a)]
                if state == nextPosition(state,a):
                    state_reward=-1
                if state_reward >= next_reward:
                    action = a
                    next_reward = state_reward
        return action


def chooseVAction(state,state_values):
        next_reward = -1
        action = ""

        if random.uniform(0,1)<= 0.2:
            action = random.choice(actions)
        else:
            for a in actions:
                state_reward = state_values[nextPosition(state,a)]
                if state == nextPosition(state,a):
                    state_reward=-1
                if state_reward >= next_reward:
                    action = a
                    next_reward = state_reward
        return action

def takeVAction(state, action):
    position = nextPosition(state,action)
    return position

def valueiteration():
    isAgentEnd = False
    state = (0,0)
    states=[state]
    self_actions =[]
    state_values = {}
    for i in range(dimx):
        for j in range(dimy):
            state_values[(i, j)] = 0

    while True:
        e=0
        if isAgentEnd:
            reward=giveReward(state)
            state_values[state] = reward 
            for s in reversed(states):
                reward = state_values[s] + gamma * (reward - state_values[s])
                e = max(e,abs(state_values[s]-reward))
                state_values[s] = reward
            if(e<theta and state==result):
                break
            isAgentEnd = False
            self_actions=[]
            state=getRandomState()
            states=[state]
        else:
            action = chooseVAction(state,state_values)
            states.append(nextPosition(state,action))
            self_actions.append(action)
            state = takeVAction(state,action)
            isAgentEnd = isEnd(state)

    resultString =""
    for i in range(0, dimx):
        for j in range(0, dimy):
            resultString +="{0} {1} {2}\n".format(i+1,j+1,actionmap[getResultVAction((i,j),state_values)])
    with open(sys.argv[2],"w+") as file:
        file.write(resultString)

def showQvalues(state_values):
    for i in range(0, dimx):
        print('-------------------------------------------------------------')
        out = '| '
        for j in range(0, dimy):
            out += str(state_values[(j,dimx-1- i)]).ljust(9) + ' | '
        print(out)
    print('-------------------------------------------------------------')

def showEnv():
    for i in range(0, dimx):
        print('---------------------')
        output = '| '
        for j in range(0, dimy):
            if environment[j][ dimx-1-i] == 1:
                token = '*'
            if environment[j][ dimx-1-i] == -2:
                token = '#'
            if environment[j] [dimx-1-i] == 0:
                token = ' '
            if environment[j] [dimx-1-i] == -1:
                token = '!'
            output += token + ' | '
        print(output)
    print('---------------------')
    

method = ""
theta = 0
gamma = 0
episodenum=0
alpha = 0
epsilon = 0
environment =[]
r_d=0 
r_o=0
r_p=0 
r_g=0
dimx = 0
dimy = 0
actions = ["N", "S", "W", "E"]
actionmap={
    "N":0,
    "E":1,
    "S":2,
    "W":3
}
result=(0,0)
with open(sys.argv[1],"r") as file:
    line = file.readline()
    line = line.strip()
    if line == "V":
        method = "V"
        line = file.readline()
        theta = float(line)
        line = file.readline()
        gamma = float(line)
    else:
        method = "Q"
        line = file.readline()
        episodenum = float(line)
        line = file.readline()
        alpha = float(line)
        line = file.readline()
        gamma = float(line)
        line = file.readline()
        epsilon = float(line)
    line=file.readline()
    dimensions = line.split(" ")
    dimx = int(dimensions[1])
    dimy = int(dimensions[0])
    environment = [[0 for k in range(dimy)] for i in range(dimx)]
    line = file.readline()
    for i in range(int(line)):
        line = file.readline()
        coor = line.split(" ")
        environment[int(coor[0])-1][int(coor[1])-1] = -2
    line = file.readline()
    for i in range(int(line)):
        line = file.readline()
        coor = line.split(" ")
        environment[int(coor[0])-1][int(coor[1])-1] = -1
    line = file.readline()
    coor = line.split(" ")
    environment[int(coor[0])-1][int(coor[1])-1] = 1
    result = (int(coor[0])-1,int(coor[1])-1)
    line = file.readline()
    rewards = line.split(" ")
    r_d = float(rewards[0])
    r_o = float(rewards[1])
    r_p = float(rewards[2])
    r_g = float(rewards[3])

reward_table = [[0 for k in range(dimy)] for i in range(dimx)]

for i in range(dimx):
    for j in range(dimy):
        if environment[i][j] == 0 :
            reward_table[i][j] = r_d
        elif environment[i][j] == -2 :
            reward_table[i][j] = r_o
        elif environment[i][j] == -1 :
            reward_table[i][j] = r_p
        elif environment[i][j] == 1 :
            reward_table[i][j] = r_g

if method=="V":
    valueiteration()
elif method=="Q":
    qlearning()
else:
    print("No such method")