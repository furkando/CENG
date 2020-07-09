import math
from sys import float_info

fname = ["parents", "has_nurs", "form", "children", "housing", "finance", "social", "health", "class"]
fvalue = [
    ["usual", "pretentious", "great_pret"],
    ["proper", "less_proper", "improper", "critical", "very_crit"],
    ["complete", "completed", "incomplete", "foster"],
    ["1", "2", "3", "more"],
    ["convenient", "less_conv", "critical"],
    ["convenient", "inconv"],
    ["nonprob", "slightly_prob", "problematic"],
    ["recommended", "priority", "not_recom"],
    ["not_recom", "recommend", "very_recom", "priority", "spec_prior"]
]
avail_ftr = [i for i in range(len(fname) - 1)]

chi_90_table = {
    4: 7.78,
    8: 13.36,
    12: 18.55,
    16: 23.54
}
epsilon = float_info.epsilon


def read_dataset(train_path, test_path):
    train, val, test = [], [], []

    with open(train_path) as f:
        for i, line in enumerate(f):
            if (i + 1) % 6 == 0: val.append(line.strip().split(","))
            else: train.append(line.strip().split(","))

    with open(test_path) as f:
        for line in f:
            test.append(line.strip().split(","))
    return train, val, test


def entropy(dataset, col, calculated=True):
    ftr_t = fvalue[-1]
    ftr_c = fvalue[col]

    if ftr_c == ftr_t:
        label = [0 for _ in range(len(ftr_t))]
        for row in dataset: label[ftr_t.index(row[-1])] += 1
        if calculated:
            entr, tot = 0, sum(label)
            for val in label:
                if val != 0: entr += (val/tot) * math.log2(val/tot)
            return -entr
        return label
    else:
        label = [[0 for _ in range(len(ftr_t))] for _ in range(len(ftr_c))]
        for row in dataset: label[ftr_c.index(row[col])][ftr_t.index(row[-1])] += 1
        if calculated:
            tmp = []
            for row in label:
                entr, tot = 0, sum(row)
                for val in row:
                    if val != 0: entr += (val / tot) * math.log2(val / tot)
                tmp.append([tot, -entr])
            return tmp
        return label


def information_gain(dataset, S, col):
    label = entropy(dataset, col)
    tot = sum([i[0] for i in label])
    avg_entr = 0
    for val in label: avg_entr += val[0]/tot * val[1]
    return S - avg_entr


def gain_ratio(dataset, S, col):
    label = entropy(dataset, col)
    tot = sum([i[0] for i in label])
    avg_entr = 0
    intr = 0
    for val in label:
        avg_entr += val[0]/tot * val[1]
        if val[0] != 0: intr += val[0]/tot * math.log2(val[0]/tot)
    return (S - avg_entr) / (-intr + epsilon)


def average_gini_index(dataset, col):
    label = entropy(dataset, col, False)
    tmp = []
    for row in label:
        gin, tot = 1, sum(row)
        if tot != 0:
            for val in row: gin -= (val/tot)**2
        tmp.append([tot, gin])
    tot = sum([i[0] for i in tmp])
    agi = 0
    for val in tmp: agi += val[0]/tot * val[1]
    return agi


def split_data(dataset, col):
    ftr_c = fvalue[col]
    tmp = [(val, []) for val in ftr_c]
    for row in dataset: tmp[ftr_c.index(row[col])][1].append(row)
    return tmp


def best_col(dataset, S, available, mode):
    if mode == 1: # Information Gain
        max = float("-inf")
        for col in available:
            ig = information_gain(dataset, S, col)
            if ig > max: max, best = ig, col
        return best
    if mode == 2: # Gain Ratio
        max = float("-inf")
        for col in available:
            gr = gain_ratio(dataset, S, col)
            if gr > max: max, best = gr, col
        return best
    if mode == 3: # Average Gini Index
        min = float("inf")
        for col in available:
            agi = average_gini_index(dataset,col)
            if agi < min: min, best = agi, col
        return best


class DecisionNode:
    def __init__(self, feature, count, parent=None):
        self.feature  = feature
        self.count    = count
        self.parent   = parent
        self.branches = []


class Leaf:
    def __init__(self, label, count, parent=None):
        self.label  = label
        self.count  = count
        self.parent = parent


def chi_squared_test(dataset, col):
    col = entropy(dataset, col, False)
    row_ratio = [x/len(dataset) for x in entropy(dataset, -1, False)]

    observed = [0 for _ in row_ratio]
    for i in range(len(row_ratio)):
        observed[i] = [x[i] for x in col]

    expected = [[] for _ in row_ratio]
    i = 0
    for val1 in row_ratio:
        for val2 in col:
            expected[i].append(val1 * sum(val2))
        i += 1

    chi = 0
    for i in range(len(row_ratio)):
        for j in range(len(col)):
           if expected[i][j] != 0:
               chi += (observed[i][j] - expected[i][j]) ** 2 / expected[i][j]

    degree = (len(row_ratio) - 1) * (len(col) - 1)
    if chi > chi_90_table[degree]: return True
    else: return False


def build_tree(dataset, available_features, mode , pre_prun=False):
    S = entropy(dataset, -1)
    label_count = entropy(dataset, -1, False)

    if S == 0: return Leaf(dataset[0][-1], label_count)
    if len(available_features) == 0:
        target = fvalue[-1][label_count.index(max(label_count))]
        return Leaf(target, label_count)

    best = best_col(dataset, S, available_features, mode)

    if pre_prun:
        if chi_squared_test(dataset, best) == False:
            target = fvalue[-1][label_count.index(max(label_count))]
            return Leaf(target, label_count)

    splits = split_data(dataset, best)
    idx = available_features.index(best)
    new_available_features = available_features[:idx] + available_features[idx + 1:]

    node = DecisionNode(fname[best], label_count)
    for sp in splits:
        if len(sp[1]) == 0:
            target = fvalue[-1][label_count.index(max(label_count))]
            node.branches.append([sp[0], Leaf(target, [0 for _ in fvalue[-1]], node)])
        else:
            child = build_tree(sp[1], new_available_features, mode, pre_prun)
            child.parent = node
            node.branches.append([sp[0], child])
    return node


def deepest_decision_node(tree, depth):
    if type(tree) is DecisionNode:
        all_leaf = True
        max = depth
        for br in tree.branches:
            if type(br[1]) is DecisionNode:
                all_leaf = False
                node, val = deepest_decision_node(br[1], depth + 1)
                if val > max: selected, max = node, val
        return (tree, depth) if all_leaf else (selected, max)
    return None, depth


def post_prune(tree, val_data):
    normal_score = score(tree, val_data)

    while True:
        node = deepest_decision_node(tree, 0)[0]

        label_count = node.count
        new_leaf = Leaf(fvalue[-1][label_count.index(max(label_count))], label_count, node.parent)

        if node.parent is None: tree = new_leaf
        else:
            for br in node.parent.branches:
                if br[1] is node:
                    br[1] = new_leaf
                    break
        if normal_score > score(tree, val_data): break
    return tree


def predict(tree, row):
    if type(tree) is Leaf: return tree.label
    else:
        col = fname.index(tree.feature)
        for br in tree.branches:
            if br[0] == row[col]: return predict(br[1], row)


def score(tree, dataset):
    count = 0
    for row in dataset:
        if predict(tree, row) == row[-1]: count += 1
    return (count/len(dataset)) * 100


def count_nodes(tree):
    if type(tree) is Leaf: return 1
    else:
        count = 1
        for br in tree.branches: count += count_nodes(br[1])
        return count


mode_map = {
    1: "Information Gain",
    2: "Gain Ratio",
    3: "Average Gini Index"
}

train, val, test = read_dataset("section3/train.txt", "section3/test.txt")
for mode in [1, 2, 3]:
    tree = build_tree(train, avail_ftr, mode)
    num_nodes = count_nodes(tree)
    test_accuracy = score(tree, test)
    print("{}\ntest accuracy: {}\nnumber of nodes: {}\n".format(mode_map[mode], test_accuracy, num_nodes))

mode = 3
tree4 = build_tree(train, avail_ftr, mode, pre_prun=True)
num_nodes = count_nodes(tree4)
test_accuracy = score(tree4, test)
print("{} Pre-prunned\ntest accuracy: {}\nnumber of nodes: {}\n".format(mode_map[mode], test_accuracy, num_nodes))


mode = 3
tree5 = post_prune(build_tree(train, avail_ftr, mode, pre_prun=True), val)
num_nodes = count_nodes(tree5)
test_accuracy = score(tree5, test)
print("{} Pre-prunned + Post-prunned\ntest accuracy: {}\nnumber of nodes: {}\n".format(mode_map[mode], test_accuracy, num_nodes))