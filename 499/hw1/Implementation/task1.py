import sys
import gzip
import pickle
import numpy as np

np.random.seed(1234)

activation = lambda t: 0 if t <= -1 else (t / 2 + 0.5 if -1 < t < 1 else 1)
derivative = lambda t: 0 if t <= -1 else (0.5 if -1 < t < 1 else 0)

sigmoid = lambda t: 1 / (1 + np.exp(-t))
sigmoid_derivative = lambda t: sigmoid(t) * (1 - sigmoid(t))


def forward(x, w1, b1, w2, b2):
    z1 = np.dot(w1, x) + b1
    a1 = np.vectorize(activation)(z1)

    z2 = np.dot(w2, a1) + b2
    a2 = sigmoid(z2)

    return z1, a1, z2, a2


# dataset1   num_epoch = 1500    eta = 0.3   accuracy = 100
# dataset2   num_epoch = 15      eta = 0.3   accuracy = 99.10
# dataset3   num_epoch = 3000    eta = 0.3   accuracy = 86
if __name__ == '__main__':
    train_path, test_path = sys.argv[1], sys.argv[2]
    num_epoch, eta = int(sys.argv[3]), 0.3

    with gzip.open(train_path) as f:
        train_data, train_labels = pickle.load(f, encoding="latin1")
        train_data, train_labels = train_data.T, train_labels.reshape(1, -1)

    with gzip.open(test_path) as f:
        test_data, test_labels = pickle.load(f, encoding="latin1")
        test_data, test_labels = test_data.T, test_labels.reshape(1, -1)

    num_examples, Ni, No = train_data.shape[1], train_data.shape[0], 1
    Nh = int((Ni + No) / 2 + 1)

    w1, b1 = np.random.randn(Nh, Ni) * 0.01, np.zeros((Nh, 1))
    w2, b2 = np.random.randn(No, Nh) * 0.01, np.zeros((No, 1))

    for i in range(num_epoch):
        z1, a1, z2, a2 = forward(train_data, w1, b1, w2, b2)

        dz2 = 2 * (a2 - train_labels) * sigmoid_derivative(z2)
        dw2 = 1 / num_examples * np.dot(dz2, a1.T)
        db2 = 1 / num_examples * np.sum(dz2, axis=1, keepdims=True)

        dz1 = np.dot(w2.T, dz2) * np.vectorize(derivative)(z1)
        dw1 = 1 / num_examples * np.dot(dz1, train_data.T)
        db1 = 1 / num_examples * np.sum(dz1, axis=1, keepdims=True)

        w2 -= eta * dw2
        b2 -= eta * db2

        w1 -= eta * dw1
        b1 -= eta * db1

    yhat = forward(test_data, w1, b1, w2, b2)[3] >= 0.5
    print("Test Accuracy: {}%".format(np.mean(yhat == test_labels) * 100))
