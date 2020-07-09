from random import seed
import math
# fix randomness; DO NOT CHANGE THIS
seed(1234)


def get_set(path):
    new_set = list()
    f = open(path)
    for i in f:
        temp_list = (i.strip("\n")).split(",")
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

    return X_train, y_train,  X_test, y_test


def get_probs(classes):
    no_examples = float(len(classes))
    unique_classes = set(classes)
    probs = dict()
    for cls in unique_classes:
        probs[cls] = classes.count(cls)
    for key in probs.keys():
        probs[key] = probs[key] / no_examples
    return probs


def get_model(training, outcome):
    classes = list(set(outcome))
    cols = len(training[0])
    likelihoods = {cls: {} for cls in classes}
    class_probs = get_probs(outcome)
    for cls in classes:
        row_indices = []
        for index, item in enumerate(outcome):
            if item == cls:
                row_indices.append(index)
        subset = list(training[i] for i in row_indices)
        r = len(subset)
        for j in range(0, cols):
            likelihoods[cls][j] = get_probs(
                list(subset[i][j] for i in range(r)))

    return {"likely": likelihoods, "classes": classes, "probs": class_probs}


def predict(model, new_sample):
    probs = {}
    for cls in model["classes"]:
        class_prob = model["probs"][cls]
        for i in range(0, len(new_sample)):
            relative_values = model["likely"][cls][i]
            if new_sample[i] in relative_values.keys():
                class_prob *= relative_values[new_sample[i]]
            else:
                class_prob = 0.0
            probs[cls] = class_prob
    best_label, best_prob = None, -1
    for cls, prob in probs.items():
        if best_label is None or prob > best_prob:
            best_prob = prob
            best_label = cls
    return best_label


def naive_bayes(train_path, test_path):
    """
    Performs naive Bayes classification
    :param train_path: path of the training set, a string
    :param test_path: path of the test set, a string
    :return: percent accuracy value for the test set, e.g., 85.43
    """

    train_set = get_set(train_path)
    test_set = get_set(test_path)

    X_train, y_train, X_test, y_test = split_set(
        train_set,
        test_set)
    model = get_model(X_train, y_train)
    true_count = 0
    for index, row in enumerate(X_test):
        label = predict(model, row)
        if label == y_test[index]:
            true_count += 1
    acc = (true_count/len(y_test))*100
    return acc


# print(naive_bayes(
#     "/Users/furkan/CENG/4-2/CENG-499/hw4/Support/task1_data/train.txt",
#     "/Users/furkan/CENG/4-2/CENG-499/hw4/Support/task1_data/test.txt"))
