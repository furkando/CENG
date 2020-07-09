import matplotlib.pyplot as plt


def single_linkage(dataset):
    min_ = (999999999, 0, 0)
    for aindex, a in enumerate(dataset):
        for iindex, i in enumerate(a):
            for jindex, j in enumerate(dataset[aindex+1:]):
                for kindex, k in enumerate(j):
                    temp = ((i[0]-k[0])**2 + (i[1]-k[1])**2)**(0.5)
                    if temp < min_[0]:
                        min_ = (temp, aindex, aindex+1+jindex)
    dataset[min_[1]] = dataset[min_[1]]+dataset[min_[2]]
    dataset.pop(min_[2])
    return dataset


def complete_linkage(dataset):
    min_ = (999999999, 0, 0)
    for aindex, a in enumerate(dataset):
        for jindex, j in enumerate(dataset[aindex+1:]):
            max_ = 0
            for iindex, i in enumerate(a):
                for kindex, k in enumerate(j):
                    temp = ((i[0]-k[0])**2 + (i[1]-k[1])**2)**(0.5)
                    if temp >= max_:
                        max_ = temp
            if max_ < min_[0]:
                min_ = (max_, aindex, aindex+1+jindex)
    dataset[min_[1]] = dataset[min_[1]]+dataset[min_[2]]
    dataset.pop(min_[2])
    return dataset


def average_linkage(dataset):
    min_ = (999999999, 0, 0)
    for aindex, a in enumerate(dataset):
        for jindex, j in enumerate(dataset[aindex+1:]):
            sum_ = 0
            for iindex, i in enumerate(a):
                for kindex, k in enumerate(j):
                    temp = ((i[0]-k[0])**2 + (i[1]-k[1])**2)**(0.5)
                    sum_ = sum_ + temp
            avg = sum_/(len(a)*len(j))
            if avg < min_[0]:
                min_ = (avg, aindex, aindex+1+jindex)
    dataset[min_[1]] = dataset[min_[1]]+dataset[min_[2]]
    dataset.pop(min_[2])
    return dataset


def centroid_linkage(dataset):
    min_ = (999999999, 0, 0)
    for aindex, a in enumerate(dataset):
        for jindex, j in enumerate(dataset[aindex+1:]):
            sum_a = (0, 0)
            sum_j = (0, 0)
            len_a = len(a)
            len_j = len(j)
            for iindex, i in enumerate(a):
                sum_a = (sum_a[0]+i[0], sum_a[1]+i[1])
            for kindex, k in enumerate(j):
                sum_j = (sum_j[0]+k[0], sum_j[1]+k[1])
            center = ((sum_a[0]/len_a-sum_j[0]/len_j)**2 +
                      (sum_a[1]/len_a-sum_j[1]/len_j)**2)**(0.5)
            if center < min_[0]:
                min_ = (center, aindex, aindex+1+jindex)
    dataset[min_[1]] = dataset[min_[1]]+dataset[min_[2]]
    dataset.pop(min_[2])
    return dataset


def hac(dataset, algorithm, k):
    dataset_copy = list(dataset)
    n = len(dataset)
    while len(dataset_copy) > k:
        algorithm(dataset_copy)
    return dataset_copy


datasets = [{"name": "dataset1", "k": 2}, {"name": "dataset2", "k": 2}, {
    "name": "dataset3", "k": 2}, {"name": "dataset4", "k": 4}]

current_dataset = datasets[3]
dataset = []

f = open("/Users/furkan/CENG/4-2/CENG-499/hw2/Datasets/section2/" +
         current_dataset["name"]+".txt")
for i in f:
    temp_list = (i.strip("\n")).split(" ")
    for index, item in enumerate(temp_list):
        try:
            temp_list[index] = float(item)
        except ValueError:
            continue
    dataset.append([(temp_list[0], temp_list[1])])
f.close()


result = hac(dataset,
             single_linkage, current_dataset["k"])

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1, facecolor="1.0")

colors = ["r", "g", "b", "c"]
for index, data in enumerate(result):
    x, y = zip(*data)
    ax.scatter(x, y, alpha=0.8, c=colors[index], edgecolors='none', s=30)

plt.title('The Centroid Linkage Criterion for ' + current_dataset["name"])
plt.show()
