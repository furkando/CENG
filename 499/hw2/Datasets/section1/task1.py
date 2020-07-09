import random

random.seed(1234)  # fix randomness


def split_dataset(dataset, n_folds):
    dataset_split = list()
    dataset_copy = list(dataset)
    fold_size = int(len(dataset) / n_folds)
    for _ in range(n_folds):
        fold = list()
        while len(fold) < fold_size:
            index = random.randrange(len(dataset_copy))
            fold.append(dataset_copy.pop(index))
        dataset_split.append(fold)
    return dataset_split


def get_accuracy(actual, predicted):
    correct = 0
    for index, item in enumerate(actual):
        if item == predicted[index]:
            correct += 1
    return correct / float(len(actual)) * 100.0


def compute_eucladian(row1, row2):
    distance = 0.0
    for index, item in enumerate(row1):
        distance += (item - row2[index])**2
    return distance**(0.5)


def get_neighbors(train_set, test_row, k):
    distances = []
    for train_row in train_set:
        dist = compute_eucladian(test_row, train_row[:-1])
        distances.append((train_row, dist))
    distances.sort(key=lambda distance: distance[1])
    neighbors = []
    for i in range(k):
        neighbors.append(distances[i][0])
    return neighbors


def predict(train_set, test_row, k):
    neighbors = get_neighbors(train_set, test_row, k)
    output_values = [row[-1] for row in neighbors]
    prediction = max(set(output_values), key=output_values.count)
    return prediction


def kNN(k, train_set, test_set):
    """
    the unweighted k-NN algorithm using Euclidean distance as the metric

    :param k: the k value, i.e, how many neighbors to consider
    :param train_set: training set, a list of lists where each nested list is a training instance
    :param test_set: test set, a list of lists where each nested list is a test instance
    :return: percent accuracy for the test set, e.g., 78.42
    """
    predictions = []
    for row in test_set:
        output = predict(train_set, row[:-1], k)
        predictions.append(output)
    return get_accuracy([row[-1] for row in test_set], predictions)


def find_best_k(train_set, test_set, num_folds):
    """
    finds the best k value by using K-fold cross validation. Try at least 10 different k values. Possible choices
    can be: 1, 3, 5, 7, 9, 11, 13, 15, 17, 19. Besides the return value, as a side effect, print each k value and
    the corresponding validation accuracy to the screen as a tuple. As an example,
    (1, 78.65)
    (3, 79.12)
    ...
    (19, 76.99)

    :param train_set: training set, a list of lists where each nested list is a training instance
    :param test_set: test set, a list of lists where each nested list is a test instance
    :param num_folds: the K value in K-fold cross validation
    :return: a tuple, best k value and percent accuracy for the test set using the best k value, e.g., (3, 80.06)
    """
    folds = split_dataset(train_set, num_folds)
    scores = list()
    temp_scores = list()
    for i in [1, 3, 5, 7, 9, 11, 13, 15, 17, 19]:
        for fold in folds:
            train_set_f = list(folds)
            train_set_f.remove(fold)
            train_set_f = sum(train_set_f, [])
            test_set_f = list()
            for row in fold:
                row_copy = list(row)
                test_set_f.append(row_copy)
            accuracy = kNN(i, train_set_f, test_set_f)
            temp_scores.append(accuracy)
        score = (i, sum(temp_scores)/len(temp_scores))
        print(score)
        scores.append(score)
    best_k = max(scores, key=lambda s: s[1])[0]
    best_acc = kNN(best_k, train_set, test_set)
    return (best_k, best_acc)


# train_set = []
# test_set = []


# f = open("/Users/furkan/CENG/4-2/CENG-499/hw2/Datasets/section1/task1_train.txt")
# for i in f:
#     temp_list = (i.strip("\n")).split(",")
#     for index, item in enumerate(temp_list):
#         try:
#             temp_list[index] = float(item)
#         except ValueError:
#             continue
#     train_set.append(temp_list)
# f.close()
# f = open("/Users/furkan/CENG/4-2/CENG-499/hw2/Datasets/section1/task1_test.txt")
# for i in f:
#     temp_list = (i.strip("\n")).split(",")
#     for index, item in enumerate(temp_list):
#         try:
#             temp_list[index] = float(item)
#         except ValueError:
#             continue
#     test_set.append(temp_list)
# f.close()

# # for i in range(1, 10):
# #     print(kNN(i, train_set, test_set))

# # kNN(7, train_set, test_set)

# print(find_best_k(train_set, test_set, 5))
