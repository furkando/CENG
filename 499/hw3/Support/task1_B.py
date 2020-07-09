from random import seed
import numpy as np
from sklearn.svm import SVC
import matplotlib.pyplot as plt

# fix randomness - DO NOT CHANGE/REMOVE THIS
seed(1234)
np.random.seed(1234)

data = np.load("task1/task1_B.npz")
X, y = data["X"], data["y"]


def meshgrid(x, y):
    xx, yy = np.meshgrid(
        np.arange(x.min() - 1,  x.max() + 1, 0.01),
        np.arange(y.min() - 1, y.max() + 1, 0.01))
    return xx, yy


c_values = [0.01, 0.1, 1,  10]
first, second = X[:, 0], X[:, 1]
xx, yy = meshgrid(first, second)
raveled = np.c_[xx.ravel(), yy.ravel()]
for key in c_values:
    svclassifier = SVC(kernel="poly", C=key)
    svclassifier.fit(X, y)
    acc = svclassifier.score(X, y)
    res = svclassifier.predict(raveled)
    res = res.reshape(xx.shape)
    fig, ax = plt.subplots()
    title = ("C = " + str(key) + ' - Accuracy: '+str(acc))
    ax.contourf(xx, yy, res, cmap=plt.cm.coolwarm, alpha=0.8)
    ax.scatter(first, second, c=y, cmap=plt.cm.coolwarm, s=20, edgecolors='k')
    ax.set_ylabel('Second Feature')
    ax.set_xlabel('First Feature')
    ax.set_xticks(())
    ax.set_yticks(())
    ax.set_title(title)
    plt.show()
