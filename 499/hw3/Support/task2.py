from random import seed

# fix randomness - DO NOT CHANGE/REMOVE THIS
seed(1234)


def get_set(path):
    new_set = list()
    f = open(path)
    for i in f:
        temp_list = (i.strip("\n")).split(",")
        for index, item in enumerate(temp_list):
            try:
                temp_list[index] = float(item)
            except ValueError:
                continue
        new_set.append(temp_list)
    f.close()
    return new_set


def split_set(train_set, test_set):
    len_train = len(train_set)
    len_test = len(test_set)

    X_train = [train_set[i][:-1] for i in range(len_train)]
    y_train = [train_set[i][-1] for i in range(len_train)]
    X_test = [test_set[i][:-1] for i in range(len_test)]
    y_test = [test_set[i][-1] for i in range(len_test)]

    return X_train, y_train, len_train, X_test, y_test, len_test


def get_rmse(X_test, y_test, len_test, B):
    rmse = 0
    for i, row in enumerate(X_test):
        predicted = predict(row, B)
        rmse += ((y_test[i] - predicted) ** 2)
    rmse = (rmse/len_test)**(0.5)
    return rmse


def predict(row, B):
    yhat = B[0]
    for i in range(len(row)):
        yhat += B[i + 1] * row[i]
    return yhat


def gradient_descent(X, y,  learning_rate, num_epochs):
    B = [0]*(len(X[0])+1)
    for epoch in range(num_epochs):
        for index, row in enumerate(X):
            yhat = predict(row, B)
            error = 2*(yhat - y[index])
            B[0] = B[0] - learning_rate * error
            for i in range(len(row)):
                B[i + 1] = B[i + 1] - learning_rate * error*row[i]
    return B


def normalize(data, length):
    num_features = len(data[0])
    features = [[data[j][i]
                 for j in range(length)] for i in range(num_features)]
    features_mins = [min(i) for i in features]
    features_maxs = [max(i) for i in features]
    normalized = [[(features[j][i] - features_mins[j]) /
                   (features_maxs[j] - features_mins[j])
                   for j in range(num_features)] for i in range(length)]
    return normalized


def linear_regression(train_path, test_path, num_epochs, learning_rate):
    """
    Performs multivariate regression
    :param train_path: path of the training set, a string
    :param test_path: path of the test set, a string
    :param num_epochs: the number of epochs, an integer
    :param learning_rate: learning rate, a float
    :return: RMSE (Root Mean Square Error) of the test set
    """
    train_set = get_set(train_path)
    test_set = get_set(test_path)

    X_train, y_train, len_train, X_test, y_test, len_test = split_set(
        train_set,
        test_set)

    X_train_norm = normalize(X_train, len_train)
    X_test_norm = normalize(X_test, len_test)

    newB = gradient_descent(X_train_norm, y_train, learning_rate, num_epochs)
    rmse = get_rmse(X_test_norm, y_test, len_test, newB)
    return rmse


print(
    linear_regression(
        "/Users/furkan/CENG/4-2/CENG-499/hw3/Support/task2/train_set.txt",
        "/Users/furkan/CENG/4-2/CENG-499/hw3/Support/task2/test_set.txt",
        1000, 0.003))
