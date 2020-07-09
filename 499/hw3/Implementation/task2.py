from random import seed
import math

# fix randomness; DO NOT CHANGE THIS
seed(1234)


def multiply(scalar, vector):
    result = []
    for v in vector: result.append(scalar * v)
    return result


def oper(vector1, vector2, operator, summed=False):
    result = []
    for a, b in zip(vector1, vector2):
        if operator == "+": result.append(a + b)
        if operator == "-": result.append(a - b)
        if operator == "*": result.append(a * b)
    return sum(result) if summed else result


def train(train_set, num_epoch, learning_rate):
    weights = [0] * (len(train_set[0]) - 1)
    bias = 0

    for i in range(num_epoch):
        for instance in train_set:
            features, y = instance[:-1], instance[-1]
            y_hat = oper(weights, features, "*", summed=True) + bias
            derr = db = 2 * (y_hat - y)
            dw = multiply(derr, features)
            weights = oper(weights, multiply(learning_rate, dw), "-")
            bias = bias - learning_rate * db
    return weights, bias


def parse_dataset(train_path, test_path):
    train_set, test_set = [], []

    with open(train_path) as f:
        for line in f:
            train_set.append(list(map(float, line.strip().split(sep=","))))

    with open(test_path) as f:
        for line in f:
            test_set.append(list(map(float, line.strip().split(sep=","))))

    return train_set, test_set


def normalize(set):
    num_features = len(set[0]) - 1
    max_values, min_values = [float("-inf")] * num_features, [float("inf")] * num_features

    for instance in set:
        for i, feature in enumerate(instance[:-1]):
            if feature < min_values[i]: min_values[i] = feature
            if feature > max_values[i]: max_values[i] = feature

    for i, instance in enumerate(set):
        set[i] = [(x - min_values[j]) / (max_values[j] - min_values[j]) for j, x in enumerate(instance[:-1])]
        set[i].append(instance[-1])
    return set


def linear_regression(train_path, test_path, num_epochs, learning_rate):
    """
    Performs multivariate regression
    :param train_path: path of the training set, a string
    :param test_path: path of the test set, a string
    :param num_epochs: the number of epochs, an integer
    :param learning_rate: learning rate, a float
    :return: RMSE (Root Mean Square Error) of the test set
    """
    train_set, test_set = parse_dataset(train_path, test_path)
    train_set, test_set = normalize(train_set), normalize(test_set)
    weights, bias = train(train_set, num_epochs, learning_rate)

    error_sum = 0
    for instance in test_set:
        features, y = instance[:-1], instance[-1]
        error = y - (oper(weights, features, "*", summed=True) + bias)
        error_sum += error ** 2
    return math.sqrt(error_sum / len(test_set))
