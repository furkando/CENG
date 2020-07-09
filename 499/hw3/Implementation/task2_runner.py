from task2 import linear_regression

map = {
    1: [4.7626, 5.0, 10000],
    2: [0.0483, 0.06, 100],
    3: [0.7398, 0.8, 300]
}


score = 0

for i in range(1, 4):
    train_path, test_path = "task2/train{}.txt".format(i), "task2/test{}.txt".format(i)
    rmse = linear_regression(train_path, test_path, num_epochs=map[i][2], learning_rate=0.001)
    if round(rmse, 4) <= map[i][1]: score += 15
    if round(rmse, 4) <= map[i][0]: score += 5
    print("RMSE = {} for Dataset {}".format(rmse, i))

print("Total Score: {}".format(score))