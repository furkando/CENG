import random
import math
from collections import Counter

random.seed(1234)  # fix randomness


def kNN(k, train_set, test_set):
    """
    the unweighted k-NN algorithm using Euclidean distance as the metric

    :param k: the k value, i.e, how many neighbors to consider
    :param train_set: training set, a list of lists where each nested list is a training instance
    :param test_set: test set, a list of lists where each nested list is a test instance
    :return: percent accuracy for the test set, e.g., 78.42
    """
    correct = total = 0
    for te_instance in test_set:
        dist_lst = []
        for i, tr_instance in enumerate(train_set):
            dist = 0
            for a, b in zip(te_instance[:-1], tr_instance[:-1]): dist += (a - b) ** 2
            dist_lst.append((i, math.sqrt(dist)))
        dist_lst.sort(key=lambda x: x[1])
        labels = Counter([train_set[best[0]][-1] for best in dist_lst[0:k]])
        label = max(labels, key=labels.get)
        correct += label == te_instance[-1]
        total += 1
    return 100 * (correct / total)


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
    fold_len = len(train_set) // num_folds
    k_list = [1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49,
              51, 53, 55, 57, 59, 61, 63, 65, 67, 69, 71, 73, 75, 77, 79, 81, 83, 85, 87, 89, 91, 93, 95, 97, 99]
    val_scores = []
    for k in k_list:
        random.shuffle(train_set)
        m = accuracy = 0
        for _ in range(num_folds):
            val_set = train_set[m:m + fold_len]
            tr_set = train_set[:m] + train_set[m + fold_len:]
            m += fold_len
            accuracy += kNN(k, tr_set, val_set)
        accuracy /= num_folds
        print((k, accuracy))
        val_scores.append((k, accuracy))
    val_scores.sort(key=lambda x: -x[1])
    best_k = val_scores[0][0]
    test_accuracy = kNN(best_k, train_set, test_set)
    return best_k, test_accuracy
