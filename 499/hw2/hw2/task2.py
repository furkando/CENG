import math
import matplotlib.pyplot as plt


class Cluster:
    def __init__(self, x, y):
        self.nodes = [(x, y)]

    def euc(self, node1, node2):
        return math.sqrt((node1[0] - node2[0])**2 + (node1[1] - node2[1])**2)

    def merge(self, other_cluster):
        self.nodes.extend(other_cluster.nodes)

    def dist(self,other_cluster, metric):
        if metric == 1: return self.single_linkage(other_cluster.nodes)
        if metric == 2: return self.complete_linkage(other_cluster.nodes)
        if metric == 3: return self.average_linkage(other_cluster.nodes)
        if metric == 4: return self.centroid_linkage(other_cluster.nodes)

    def single_linkage(self, other_nodes):
        dist = float("inf")
        for node1 in self.nodes:
            for node2 in other_nodes:
                d = self.euc(node1, node2)
                if d < dist: dist = d
        return dist

    def complete_linkage(self, other_nodes):
        dist = float("-inf")
        for node1 in self.nodes:
            for node2 in other_nodes:
                d = self.euc(node1, node2)
                if d > dist: dist = d
        return dist

    def average_linkage(self, other_nodes):
        dist = 0
        for node1 in self.nodes:
            for node2 in other_nodes:
                dist += self.euc(node1, node2)
        return dist / (len(self.nodes) * len(other_nodes))

    def centroid_linkage(self, other_nodes):
        med1 = [0, 0]
        med2 = [0, 0]
        for node1 in self.nodes:
            med1[0] += node1[0]
            med1[1] += node1[1]
        med1 = [med1[0]/len(self.nodes), med1[1]/len(self.nodes)]
        for node2 in other_nodes:
            med2[0] += node2[0]
            med2[1] += node2[1]
        med2 = [med2[0]/len(other_nodes), med2[1]/len(other_nodes)]
        return self.euc(med1, med2)


def read_dataset(file):
    with open(file) as f:
        lst = []
        for line in f:
            x, y = line.split()
            lst.append(Cluster(float(x), float(y)))
    return lst


def hac(dataset, rem_clusters, metric):
    while len(dataset) != rem_clusters:
        table = [ [0 for _ in range(len(dataset))] for _ in range(len(dataset)) ]
        x, y, min = 0, 0, float("inf")
        for i in range(len(dataset) - 1):
            for j in range(i + 1, len(dataset)):
                d = dataset[i].dist(dataset[j], metric)
                if d < min: x, y, min = i, j, d
                table[i][j] = d
        dataset[x].merge(dataset[y])
        del dataset[y]


criteria_map = {
    1: "Single Linkage",
    2: "Complete Linkage",
    3: "Average Linkage",
    4: "Centroid Linkage"
}


def plot(dataset, filename, criteria):
    global criteria_map
    for cluster in dataset:
        x = []
        y = []
        for node in cluster.nodes:
            x.append(node[0])
            y.append(node[1])
        plt.scatter(x, y)

    plt.title("{} - {}".format(filename, criteria_map[criteria]))
    plt.show()


k_map = {
    "dataset1": 2,
    "dataset2": 2,
    "dataset3": 2,
    "dataset4": 4,
}


filename = "dataset4"
k = k_map[filename]

for i in range(4):
    dataset = read_dataset("section2/{}.txt".format(filename))
    hac(dataset, k, i + 1)
    plot(dataset, filename, i + 1)
