import math
import pprint
import matplotlib.pyplot as plt


def get_node_count(tree, flag):
    node_count = 0

    for i in tree:
        if "output" in tree[i]:
            node_count += 1
            continue
        if i == "values":
            continue
        if flag:
            node_count += 1 + get_node_count(tree[i], not flag)
        else:
            node_count += get_node_count(tree[i], not flag)
    return node_count


def post_prune(tree, validation_set):
    if isinstance(tree, dict):
        key = list(tree.keys())
        prunable = True
        for leaf in tree[key[0]]:
            if leaf == "values" or "output" in tree[key[0]][leaf]:
                continue
            if isinstance(tree[key[0]][leaf], dict):
                prunable = False
                break

        if prunable:
            outputs = [i[-1] for i in validation_set]
            all_unique = set(output)
            if len(outputs) == 0:
                return
            maxl = max(set(outputs), key=outputs.count)
            maxi = outputs.count(maxl)
            temp = 0
            for i in tree[key[0]]:
                if leaf == "values" or "output" in tree[key[0]][i]:
                    continue
                temp += len([v for v in validation_set if v[features[key[0]]
                                                            [0]] == i and v[-1] == tree[key[0]][i]])
            if maxi >= temp:
                output_counts = tree[key[0]]["values"]
                tree = {"output": maxl, "values": output_counts}
            return tree
        else:
            for i in tree[key[0]]:
                if leaf == "values" or "output" in tree[key[0]][i]:
                    continue
                tree[key[0]][i] = post_prune(
                    tree[key[0]][i], [v for v in validation_set if v[features[key[0]][0]] == i])
            return tree
    else:
        return tree


def get_accuracy(tree, test_set):
    total = 0
    true = 0
    for i in test_set:
        temp_tree = dict(tree)
        while(True):
            keylist = list(temp_tree.keys())
            temp_tree = temp_tree[keylist[0]][i[features[keylist[0]][0]]]
            if "output" in temp_tree:
                total += 1
                if temp_tree["output"] == i[-1]:
                    true += 1
                break
    print("Accuracy for "+key + " is: " + str(true*100/total) + "%")
    print()


def avg_gini_index(dataset, attribute_index):
    target_variables = set(i[-1] for i in dataset)
    # target_variables = set(output)
    variables = set(i[attribute_index] for i in dataset)
    # variables = set(features[i] for i in features if i[0] == attribute_index)

    avg_gini = 0
    for variable in variables:
        gini_each_feature = 0
        for target_variable in target_variables:
            num = [i[-1] == target_variable and i[attribute_index]
                   == variable for i in dataset].count(True)
            den = [i[attribute_index] == variable for i in dataset].count(True)
            fraction = num/(den+eps)
            gini_each_feature += (fraction**2)
        fraction2 = den/len(dataset)
        avg_gini += (fraction2*(1-gini_each_feature))
    return avg_gini


def ig(entropy, e_attribute):
    return(entropy-e_attribute)


def att_ent(dataset, attribute_index):
    target_variables = set(i[-1] for i in dataset)
    variables = set(i[attribute_index] for i in dataset)
    # variables = set(features[i] for i in features if i[0] == attribute_index)

    entropy_attribute = 0
    for variable in variables:
        entropy_each_feature = 0
        for target_variable in target_variables:
            num = [i[-1] == target_variable and i[attribute_index]
                   == variable for i in dataset].count(True)
            den = [i[attribute_index] == variable for i in dataset].count(True)
            fraction = num/(den+eps)
            entropy_each_feature += -fraction*math.log(fraction+eps)
        fraction2 = den/len(dataset)
        entropy_attribute += -fraction2*entropy_each_feature

    return(abs(entropy_attribute))


def intrinsic(dataset, attribute_index):
    target_variables = set(i[-1] for i in dataset)
    variables = set(i[attribute_index] for i in dataset)
    # variables = set(features[i] for i in features if i[0] == attribute_index)
    dataset_len = len(dataset)
    intr = 0
    for variable in variables:
        den = [i[attribute_index] == variable for i in dataset].count(True)
        intr += (den/dataset_len)*math.log2(den/dataset_len)
    return -intr


def find_best_ig(dataset):
    Entropy_att = []
    IG = {}

    for index, key in enumerate(features.keys()):
        IG[key] = ent(dataset)-att_ent(dataset, index)
    return max(IG, key=IG.get)


def find_best_gr(dataset):
    Entropy_att = []
    GR = {}

    for index, key in enumerate(features.keys()):
        GR[key] = (ent(dataset)-att_ent(dataset, index)) / \
            (intrinsic(dataset, index)+eps)
    return max(GR, key=GR.get)


def find_best_ag(dataset):
    Entropy_att = []
    AG = {}

    for index, key in enumerate(features.keys()):
        AG[key] = avg_gini_index(dataset, index)
    return min(AG, key=AG.get)


def ent(dataset):
    unique = set(i[-1] for i in dataset)
    # unique = set(output)

    entropy = 0

    for value in unique:
        fraction = [i[-1] for i in dataset].count(value)/len(dataset)
        entropy -= fraction*math.log(fraction+eps)

    return entropy


def get_sub(dataset, attributeIndex, value):
    return [i for i in dataset if i[attributeIndex] == value]


def buildTree(dataset, algorithm, tree=None, prune=False):
    global node_count
    node = algorithm(dataset)

    # unique = set(i[-1] for i in dataset)

    att_value = set(features[node][1])
    # att_value = set(i[features[node][0]] for i in dataset)
    outputs = [i[-1] for i in dataset]
    output_counts = [outputs.count(i) for i in output]
    if prune:
        chi = 0
        attp = {}
        exp = {}
        for value in att_value:
            attp[value] = {}
            exp[value] = {}
            for out in output:
                sub = get_sub(dataset, features[node][0], value)
                total = len(get_sub(dataset, -1, out))
                attp[value][out] = len(get_sub(sub, -1, out))
                exp[value][out] = total*len(sub)/len(dataset)
        for key, value in exp.items():
            for key2, value2 in value.items():
                chi += ((value2-attp[key][key2])**2)/(value2+eps)
        if chi < chi_square_90[(len(output)-1)*(len(att_value)-1)]:
            # temp = [i[-1] for i in dataset]
            return {"output": max(set(
                outputs), key=outputs.count), "values": ','.join(str(e) for e in output_counts)}

    if tree is None:
        tree = {}
        tree[node] = {"values": ','.join(str(e) for e in output_counts)}
    for value in att_value:
        subtable = get_sub(dataset, features[node][0], value)
        sub_outs = [i[-1] for i in subtable]
        output_counts = [sub_outs.count(i) for i in output]
        unique = list(set(sub_outs))
        node_count += 1
        if len(unique) == 1:
            tree[node][value] = {"output": unique[0], "values": ','.join(
                str(e) for e in output_counts)}
        elif subtable == []:
            # temp = [i[-1] for i in dataset]
            tree[node][value] = {"output": max(set(
                outputs), key=outputs.count), "values": ','.join(str(e) for e in output_counts)}
        else:
            tree[node][value] = buildTree(subtable, algorithm, None, prune)

    return tree


train_set = []
test_set = []


f = open("/Users/furkan/CENG/4-2/CENG-499/hw2/Datasets/section3/train.txt")
for i in f:
    temp_list = (i.strip("\n")).split(",")
    train_set.append(temp_list)
f.close()

f = open("/Users/furkan/CENG/4-2/CENG-499/hw2/Datasets/section3/test.txt")
for i in f:
    temp_list = (i.strip("\n")).split(",")
    test_set.append(temp_list)
f.close()

features = {
    'parents': [0, {'usual', 'great_pret', 'pretentious'}],
    'has_nurs': [1, {'critical', 'less_proper', 'proper', 'very_crit', 'improper'}],
    'form': [2, {'incomplete', 'complete', 'completed', 'foster'}],
    'children': [3, {'1', '2', '3', 'more'}],
    'housing': [4, {'less_conv', 'convenient', 'critical'}],
    'finance': [5, {'convenient', 'inconv'}],
    'social': [6, {'slightly_prob', 'nonprob', 'problematic'}],
    'health': [7, {'recommended', 'not_recom', 'priority'}]}

output = ['not_recom', 'recommend', 'very_recom', 'priority', 'spec_prior']

chi_square_10 = {1: 0.016, 2: 0.211, 3: 0.584, 4: 1.064, 6: 2.204, 8: 3.490,
                 9: 4.168, 10: 4.865, 12: 6.304, 15: 8.547, 16: 11.912}
chi_square_90 = {1: 2.71, 2: 4.61, 3: 6.25, 4: 7.78, 6: 10.64, 8: 13.36,
                 9: 14.68, 10: 15.99, 12: 18.55, 15: 22.31, 16: 23.54}
chi_square_99 = {4: 13.28, 8: 20.09,
                 12: 26.22,  16: 32}

eps = 10**(-100)

# algorithms = {"Average Gini Index": find_best_ag}
algorithms = {"Information Gain": find_best_ig,
              "Gain Ratio": find_best_gr, "Average Gini Index": find_best_ag}

for key, algorithm in algorithms.items():
    node_count = 1
    t = buildTree(train_set, algorithm, None, False)
    print("Node Count for "+key + " is: " + str(node_count))
    # pprint.pprint(t)
    get_accuracy(t, test_set)

node_count = 1
t = buildTree(train_set, find_best_ag, None, True)
print("Node Count for pre prunned Average Gini Index is: " + str(node_count))
get_accuracy(t, test_set)
# pprint.pprint(t)

validation_set = train_set[int(3*len(train_set)/4):]
post_prune(t, validation_set)
print("Node Count for post prunned Average Gini Index is: " +
      str(get_node_count(t, True)))
get_accuracy(t, test_set)
# pprint.pprint(t)
