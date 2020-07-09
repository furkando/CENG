import matplotlib.pyplot as plt
from sklearn.svm import SVC
from random import seed
import numpy as np


# fix randomness - DO NOT CHANGE/REMOVE THIS
seed(1234)
np.random.seed(1234)

data = np.load("task1/task1_B.npz")
X, y = data["X"], data["y"]


def plotSVC(ax, clf, title):
    x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
    y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
    h = .01
    xx, yy = np.meshgrid(
        np.arange(x_min, x_max, h),
        np.arange(y_min, y_max, h))
    plt.subplot(1, 1, 1)
    Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)
    plt.contourf(xx, yy, Z, cmap=plt.cm.coolwarm, alpha=0.8)
    plt.scatter(
        X[:, 0],
        X[:, 1],
        c=y, cmap=plt.cm.coolwarm, s=20, edgecolors='k')
    plt.ylabel('First Feature')
    plt.xlabel('Second Feature')
    plt.xlim(xx.min(), xx.max())
    plt.title(title)


fignum = 1
for c in (0.01, 0.1, 1, 10):
    clf = SVC(C=c, kernel="poly")
    clf.fit(X, y)
    fig, ax = plt.subplots()
    title = "C = " + str(c) + " - Accuracy: " + str(round(clf.score(X, y), 2))
    plotSVC(ax, clf, title)
    fignum = fignum + 1
    plt.show()
