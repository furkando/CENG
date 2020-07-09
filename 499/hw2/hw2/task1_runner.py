from task1 import kNN, find_best_k


def dataset_reader(path):
    lst = []
    with open(path) as f:
        for line in f:
            splits = line.strip().split(sep=",")
            for i in range(len(splits) - 1):
                splits[i] = float(splits[i])
            lst.append(splits)
    return lst


train_path = "section1/train.txt"
test_path = "section1/test.txt"
train_set, test_set = dataset_reader(train_path), dataset_reader(test_path)

# calling the function kNN
k = 11
test_accuracy = kNN(k, train_set, test_set)
print("k: {} | test accuracy: {} for the dataset".format(k, test_accuracy), "\n\n")

# calling the function find_best_k
num_folds = 5
best_k, test_accuracy = find_best_k(train_set, test_set, num_folds)
print("best k is {} | test accuracy: {} for the dataset".format(best_k, test_accuracy), "\n\n")
