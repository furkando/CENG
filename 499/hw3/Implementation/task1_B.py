from random import seed
import matplotlib.pyplot as plt
import numpy as np
from sklearn import svm
from sklearn.metrics import accuracy_score


# fix randomness - DO NOT CHANGE/REMOVE THIS
seed(1234)
np.random.seed(1234)


def plot_decision_boundary(X, y, clf, title):
    X0, X1, h = X[:, 0], X[:, 1], .02
    x_min, x_max = X0.min() - 1, X0.max() + 1
    y_min, y_max = X1.min() - 1, X1.max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))

    ax = plt.gca()
    Z = clf.predict(np.c_[xx.ravel(), yy.ravel()]).reshape(xx.shape)
    ax.contourf(xx, yy, Z, cmap=plt.cm.coolwarm, alpha=0.8)

    ax.scatter(X0, X1, c=y, cmap=plt.cm.coolwarm, s=20, edgecolors='k')
    ax.set_xlim(xx.min(), xx.max())
    ax.set_ylim(yy.min(), yy.max())
    ax.set_xlabel('First Feature')
    ax.set_ylabel('Second Feature')
    ax.set_xticks(())
    ax.set_yticks(())

    ax.set_title(title + " - Accuracy: {}".format(accuracy_score(y, clf.predict(X))))

    plt.show()


data = np.load("task1/task1_B.npz")
X, y = data["X"], data["y"]


model = svm.SVC(kernel="poly", C=0.01)
clf = model.fit(X, y)
plot_decision_boundary(X, y, clf, "C = 0.01")

model = svm.SVC(kernel="poly", C=0.1)
clf = model.fit(X, y)
plot_decision_boundary(X, y, clf, "C = 0.1")

model = svm.SVC(kernel="poly", C=1)
clf = model.fit(X, y)
plot_decision_boundary(X, y, clf, "C = 1")

model = svm.SVC(kernel="poly", C=10)
clf = model.fit(X, y)
plot_decision_boundary(X, y, clf, "C = 10")
