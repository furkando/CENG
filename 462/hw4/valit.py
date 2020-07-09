import numpy as np

# global variables
BOARD_ROWS = 5
BOARD_COLS = 5
WIN_STATE = (4, 4)
LOSE_STATE = (4, 3)
START = (0, 0)
DETERMINISTIC = True


class State:
    def __init__(self, state=START):
        self.board = np.zeros([BOARD_COLS, BOARD_ROWS])
        self.board[2, 0] = -1
        self.board[2, 1] = -1
        self.board[2, 3] = -1
        self.board[2, 4] = -1
        self.state = state
        self.isEnd = False
        self.determine = DETERMINISTIC

    def giveReward(self):
        if self.state == WIN_STATE:
            return 1
        elif self.state == LOSE_STATE:
            return -1
        else:
            return 0

    def isEndFunc(self):
        if (self.state == WIN_STATE) or (self.state == LOSE_STATE):
            self.isEnd = True

    def nxtPosition(self, action):
        if self.determine:
            if action == "up":
                nxtState = (self.state[0] , self.state[1]+1)
            elif action == "down":
                nxtState = (self.state[0] , self.state[1]-1)
            elif action == "left":
                nxtState = (self.state[0]-1, self.state[1] )
            else:
                nxtState = (self.state[0]+1, self.state[1] )

            if (nxtState[0] >= 0) and (nxtState[0] < BOARD_COLS):
                if (nxtState[1] >= 0) and (nxtState[1] < BOARD_ROWS):
                    if self.board[nxtState[0],nxtState[1]] != -1:
                        return nxtState
            return self.state

    def showBoard(self):
        self.board[self.state] = 1
        for i in range(0, BOARD_ROWS):
            print('-----------------')
            out = '| '
            for j in range(0, BOARD_COLS):
                if self.board[j, BOARD_ROWS-1-i] == 1:
                    token = '*'
                if self.board[j, BOARD_ROWS-1-i] == -1:
                    token = 'z'
                if self.board[j, BOARD_ROWS-1-i] == 0:
                    token = '0'
                out += token + ' | '
            print(out)
        print('-----------------')


# Agent of player

class Agent:

    def __init__(self):
        self.states = []
        self.actions = ["up", "down", "left", "right"]
        self.State = State()
        self.lr = 0.9
        self.exp_rate = 0.3
        self.found = False
        # initial state reward
        self.state_values = {}
        for i in range(BOARD_ROWS):
            for j in range(BOARD_COLS):
                self.state_values[(i, j)] = 0  # set initial value to 0

    def chooseAction(self):
        # choose action with most expected value
        mx_nxt_reward = -1
        action = ""

        if np.random.uniform(0, 1) <= self.exp_rate:
            action = np.random.choice(self.actions)
        else:
            for a in self.actions:
                nxt_reward = self.state_values[self.State.nxtPosition(a)]
                if self.State.state == self.State.nxtPosition(a):
                    print("nasil")
                    nxt_reward=-1
                if nxt_reward >= mx_nxt_reward:
                    action = a
                    mx_nxt_reward = nxt_reward
        return action

    def takeAction(self, action):
        position = self.State.nxtPosition(action)
        return State(state=position)

    def reset(self):
        self.states = []
        self.State = State()

    def play(self, rounds=10):
        i = 0
        while True:
            e = 0
            # to the end of game back propagate reward
            if self.State.isEnd:
                # back propagate
                reward = self.State.giveReward()
                if reward==1:
                    print(i)
                    self.found = True
                # explicitly assign end state to reward values
                self.state_values[self.State.state] = reward  
                print("Game End Reward", reward)
                for s in reversed(self.states):
                    reward = self.state_values[s] + self.lr * (reward - self.state_values[s])
                    e = max(e,abs(self.state_values[s]- round(reward, 5)))
                    self.state_values[s] = round(reward, 5)
                print(self.showValues())
                print(e)
                if(e<0.01):
                    print("bitti aq")
                    break
                self.reset()
                i += 1
            else:
                action = self.chooseAction()
                # append trace
                self.states.append(self.State.nxtPosition(action))
                # print("current position {} action {}".format(self.State.state, action))

                self.State = self.takeAction(action)

                self.State.isEndFunc()
                # print("nxt state", self.State.state)
                # print("---------------------")
                # self.State.showBoard()


    def showValues(self):
        for i in range(0, BOARD_ROWS):
            print('----------------------------------')
            out = '| '
            for j in range(0, BOARD_COLS):
                out += str(self.state_values[(j,BOARD_COLS-1- i)]).ljust(6) + ' | '
            print(out)
        print('----------------------------------')


if __name__ == "__main__":
    ag = Agent()
    ag.play()
    # print(ag.showValues())
    # print(ag.State.showBoard())
