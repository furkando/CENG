import numpy as np
import pickle
import sys
import gzip

np.random.seed(1234)


def sigmoid(x):
    return 1/(1+np.exp(-x))


def hidden(x):
    a = np.array(x, copy=True)
    return np.piecewise(a, [a <= -1, (a > -1) & (a < 1), a >= 1], [lambda a: 0, lambda a: ((a / 2) + 0.5), lambda a: 1])


def sigmoid_backward(dX, x):
    sig = sigmoid(x)
    return dX * sig * (1 - sig)


def hidden_backward(dX, x):
    dZ = np.array(x, copy=True)
    dZ[x <= -1] = 0
    dZ[x >= 1] = 0
    dZ[np.logical_and(x > -1, x < 1)] = 0.5
    return dX * dZ


def loss_function(Y, Y_hat):
    return np.mean(np.square(Y-Y_hat))


def forward_propagation_helper(A_prev, W, b, activation):
    Z = np.dot(W, A_prev) + b
    if activation is "sigmoid":
        return Z, sigmoid(Z)
    elif activation is "hidden":
        return Z, hidden(Z)


def forward_propagation(X, params, neural_network):
    mem_to_cache = {}
    A = X

    for index, layer in enumerate(neural_network):
        A_prev = A
        active_function = layer["function"]
        W = params["W" + str(index + 1)]
        b = params["b" + str(index + 1)]
        Z, A = forward_propagation_helper(
            A_prev, W, b, active_function)
        mem_to_cache["A" + str(index)] = A_prev
        mem_to_cache["Z" + str(index + 1)] = Z

    return A, mem_to_cache


def backward_propagation_helper(dA, W, b, Z, A_prev, activation):
    N = A_prev.shape[1]

    if activation is "hidden":
        backward_activation_func = hidden_backward
    elif activation is "sigmoid":
        backward_activation_func = sigmoid_backward

    dZ = backward_activation_func(dA, Z)
    dW = np.dot(dZ, A_prev.T) / N
    db = np.sum(dZ, axis=1, keepdims=True) / N
    dA_prev = np.dot(W.T, dZ)

    return dA_prev, dW, db


def backward_propagation(Y_hat, Y, mem_to_cache, params, neural_network):
    gd_values = {}

    dA_prev = -2*(np.subtract(Y, Y_hat))

    for index_prev, layer in reversed(list(enumerate(neural_network))):
        index = index_prev + 1
        active_function = layer["function"]
        dA = dA_prev
        A_prev = mem_to_cache["A" + str(index_prev)]
        Z = mem_to_cache["Z" + str(index)]
        W = params["W" + str(index)]
        b = params["b" + str(index)]
        dA_prev, dW, db = backward_propagation_helper(
            dA, W, b, Z, A_prev, active_function)
        gd_values["dW" + str(index)] = dW
        gd_values["db" + str(index)] = db
    return gd_values


def update_values(params, gd_values, neural_network):
    for index_prev, layer in enumerate(neural_network):
        index = index_prev + 1
        params["W" + str(index)
               ] -= gd_values["dW" + str(index)]
        params["b" + str(index)
               ] -= gd_values["db" + str(index)]

    return params


def get_accuracy(Y_hat, Y):
    probs_ = np.copy(Y_hat)
    probs_[probs_ > 0.5] = 1
    probs_[probs_ <= 0.5] = 0
    Y_hat_ = probs_
    return (Y_hat_ == Y).all(axis=0).mean()


with gzip.open(sys.argv[1]) as f:
    data, labels = pickle.load(f, encoding='latin1')

with gzip.open(sys.argv[2]) as f:
    test_data, test_labels = pickle.load(f, encoding='latin1')

X_test = np.array(test_data).T
Y_test = np.array([np.array(test_labels).T])

input_dim = X_test.shape[0]
hidden_dim = int((input_dim+1)/2)

neural_network = [
    {"input_dimension": input_dim, "output_dimension": hidden_dim, "function": "hidden"},
    {"input_dimension": hidden_dim, "output_dimension": 1, "function": "sigmoid"},
]

number_of_layers = len(neural_network)
params = {}

for index, layer in enumerate(neural_network):
    layer_index = index + 1
    layer_input_size = layer["input_dimension"]
    layer_output_size = layer["output_dimension"]

    params['W' + str(layer_index)] = np.random.randn(
        layer_output_size, layer_input_size) * 0.01
    params['b' + str(layer_index)] = np.zeros((layer_output_size, 1))


X = np.array(data).T
Y = np.array([np.array(labels).T])
for i in range(int(sys.argv[3])):
    Y_hat, cache = forward_propagation(
        X, params, neural_network)
    lost = loss_function(Y_hat, Y)
    print("Loss is: "+str(lost))
    gd_values = backward_propagation(
        Y_hat, Y, cache, params, neural_network)
    params = update_values(params, gd_values, neural_network)

Y_hat, _ = forward_propagation(X_test, params, neural_network)
print("Accuracy is: "+str(get_accuracy(Y_hat, Y_test)*100)+"%")
