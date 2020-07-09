from copy import deepcopy


class Puzzle:
    def __init__(self, data, parent, emptyIndex, gridSize, answer, maxDepth):
        self.maxDepth = maxDepth
        self.answer = answer
        self.gridSize = gridSize
        self.data = data
        self.parent = parent
        self.emptyIndex = emptyIndex
        self.gVal = 0
        self.hVal = 0
        self.fVal = 0

    def add_child(self, obj):
        self.children.append(obj)

    def move_right(self):
        temp = deepcopy(self.data)
        (key, value) = self.emptyIndex
        if(value == 0):
            return None
        temp[key][value] = temp[key][value-1]
        temp[key][value-1] = "_"
        newObj = Puzzle(temp, self, (key, value-1),
                        self.gridSize, self.answer, self.maxDepth)
        return newObj

    def move_left(self):
        temp = deepcopy(self.data)
        (key, value) = self.emptyIndex
        if(value == self.gridSize-1):
            return None
        temp[key][value] = temp[key][value+1]
        temp[key][value+1] = "_"
        newObj = Puzzle(temp, self, (key, value+1),
                        self.gridSize, self.answer, self.maxDepth)
        return newObj

    def move_up(self):
        temp = deepcopy(self.data)
        (key, value) = self.emptyIndex
        if(key == self.gridSize-1):
            return None
        temp[key][value] = temp[key+1][value]
        temp[key+1][value] = "_"
        newObj = Puzzle(temp, self, (key+1, value),
                        self.gridSize, self.answer, self.maxDepth)
        return newObj

    def move_down(self):
        temp = deepcopy(self.data)
        (key, value) = self.emptyIndex
        if(key == 0):
            return None
        temp[key][value] = temp[key-1][value]
        temp[key-1][value] = "_"
        newObj = Puzzle(temp, self, (key-1, value),
                        self.gridSize, self.answer, self.maxDepth)
        return newObj

    def manhattanDistance(self, obj):
        distance = 0
        lenght = self.gridSize
        for i in range(0, lenght):
            for j in range(0, lenght):
                if(obj[i][j] != "_"):
                    index = self.answer.index(obj[i][j])
                    (row, column) = divmod(index, lenght)
                    x = abs(row-i)
                    y = abs(column-j)
                    distance += x+y
        return distance


def getBest(frontierList):
    fVal = float("inf")
    bestIndex = 0
    for index, i in enumerate(frontierList):
        if (i.fVal < fVal):
            fVal = i.fVal
            bestIndex = index
    return frontierList[bestIndex], bestIndex


def dfs(node, limit, depth):
    depth += 1
    # print("node")
    # print(node.data)
    # print("answer")
    # print(node.answer)
    if node.data == answer:
        # print(123)
        return node, node.fVal
    if node.fVal > limit:
        return node, node.fVal
    temp = []
    tile = node.move_up()
    if(tile != None):
        temp.append(tile)
    tile = node.move_down()
    if(tile != None):
        temp.append(tile)
    tile = node.move_left()
    if(tile != None):
        temp.append(tile)
    tile = node.move_right()
    if(tile != None):
        temp.append(tile)
    minimum = node.maxDepth
    for i in temp:
        i.gVal = i.manhattanDistance(i.data)
        i.fVal = i.gVal + node.fVal
        # print(i.parent)
        # print(i.fVal)
        # print(limit)
        ret, newlimit = dfs(i, limit, depth)
        if newlimit < minimum:
            minimum = newlimit
        if ret.data == answer:
            # print(ret.data)
            # print(ret.parent.data)
            # print(ret.parent.parent.data)
            # print(ret.parent.parent.parent.data)
            return ret, ret.fVal
    return node, minimum


def SolvePuzzleIDA(puzzle):
    maxDepth = puzzle.maxDepth
    depth = 0
    manhattan = puzzle.manhattanDistance(puzzle.data)
    puzzle.gVal = manhattan
    puzzle.fVal = 0
    node = None
    while manhattan < maxDepth:
        node, tempLimit = dfs(puzzle, manhattan, depth)
        # node, tempLimit = dfsHeuristic2(puzzle, manhattan)
        # print("here")
        # print(node.data)
        # print(manhattan)
        # print(maxDepth)
        manhattan = tempLimit + 1
        # if (node!=None):
        if node.data == answer:
            # print("SUCCESS")
            return node
    return None


def solvePuzzleA(puzzle):
    depth = 0
    frontierList = []
    exploredList = []
    frontierList.append(puzzle)
    while (frontierList and depth <= puzzle.maxDepth):
        bestMove, bestIndex = getBest(frontierList)
        depth += 1
        if(bestMove.data == answer):
            # print(depth)
            # print("SUCCESS")
            return bestMove
        frontierList.pop(bestIndex)
        exploredList.append(bestMove)
        moveList = []
        tile = bestMove.move_up()
        if(tile != None):
            moveList.append(tile)
        tile = bestMove.move_down()
        if(tile != None):
            moveList.append(tile)
        tile = bestMove.move_left()
        if(tile != None):
            moveList.append(tile)
        tile = bestMove.move_right()
        if(tile != None):
            moveList.append(tile)
        # print(moveList)
        for move in moveList:
            flag = False
            for explored in exploredList:
                if (explored == move):
                    flag = True
                    break
            if (flag == False):
                cost = bestMove.gVal + 1
                current = False
                for frontier in frontierList:
                    if (frontier == move):
                        bestIndex = frontierList.index(frontier)
                        current = True
                        if (cost < frontierList[bestIndex].gVal):
                            frontierList[bestIndex].gVal = cost
                            # print("asd")
                            frontierList[bestIndex].parent = bestMove
                if (current == False):
                    # print("asd")
                    move.gVal = cost
                    move.hVal = move.manhattanDistance(move.data)
                    move.fVal = move.gVal+move.hVal
                    frontierList.append(move)
    # print(depth)
    return None


algoName = input()
maxDepth = int(input())
gridSize = int(input())
grid = []
answer = []
for i in range(0, gridSize):
    grid.append(input().split(" "))
for i in range(0, gridSize):
    answer.append(input().split(" "))

flat_answer = []
for sublist in answer:
    for item in sublist:
        flat_answer.append(item)

for i in range(0, gridSize):
    for j in range(0, gridSize):
        if (grid[i][j] == "_"):
            emptyIndex = (i, j)

newPuzzle = Puzzle(grid, None, emptyIndex, gridSize, flat_answer, maxDepth)
if (algoName == "A*"):
    answer = solvePuzzleA(newPuzzle)
elif (algoName == "IDA*"):
    answer = SolvePuzzleIDA(newPuzzle)
    # print(answer.parent.data)
if (answer == None):
    print("FAILURE")
else:
    print("SUCCESS")
    answerList = []
    answerList.insert(0, answer)
    temp = answer.parent
    while (temp):
        answerList.insert(0, temp)
        temp = temp.parent
    for answers in answerList:
        print()
        for i in range(gridSize):
            for j in range(gridSize):
                print(answers.data[i][j], end=" ")
            print()
