from copy import deepcopy


class Clause:
    def __init__(self, name, const, negate, child=None):
        self.name = name
        self.const = const
        self.negate = negate
        self.child = child


def cnfparser(clauselist):
    cnf = []
    for x in clauselist:
        splitted = x.split("+")
        clauses = []
        for y in splitted:
            spy = y.split(",")
            clause = recparser(spy)
            clauses.append(clause)
        cnf.append(clauses)
    return cnf


def recparser(clauselist):
    if len(clauselist) == 0:
        return None
    index = clauselist[0].find("(")
    index2 = clauselist[0].find(")")
    if index2 == -1:
        index2 = len(clauselist[0])
    index3 = 0
    varlist = []
    for i in clauselist:
        if i.find("(")+1 and index3:
            break
        index4 = i.find("(")
        index5 = i.find(")")
        if index4 == -1 and index5 == -1:
            varlist.append(i)
        elif index4 == -1 and index5 != -1:
            varlist.append(i[:index5])
        elif index4 != -1 and index5 == -1:
            varlist.append(i[index4+1:])
        else:
            varlist.append(i[index4+1:index5])
        index3 += 1
    if clauselist[0][0] == "~":
        clause = Clause(clauselist[0][1:index],
                        varlist, True)
        clause.child = recparser(clauselist[index3:])
        return clause
    else:
        clause = Clause(clauselist[0][0:index],
                        varlist, False)
        clause.child = recparser(clauselist[index3:])
        return clause


def clausecreateor(clauses):
    result = ""
    lenght = len(clauses)
    for clindex, i in enumerate(clauses):
        index = 0
        while i:
            index += 1
            if i.negate:
                result += "~"
            result += i.name
            result += "("
            for jindex, j in enumerate(i.const):
                result += j
                if jindex+1 != len(i.const):
                    result += ","
            i = i.child
            if i:
                result += ","
        for j in range(index):
            result += ")"
        if lenght > 1 and clindex != lenght-1:
            result += "+"
    return result


def clausereducer(liste2, clause, liste, reducer):
    liste.remove(reducer)
    liste2.remove(clause)
    cliste1 = deepcopy(liste)
    cliste2 = deepcopy(liste2)
    for i in cliste1:
        for index, j in enumerate(i.const):
            for index2, k in enumerate(reducer.const):
                if(j == k):
                    i.const[index] = clause.const[index2]
        childi = i.child
        while childi:
            for index, j in enumerate(childi.const):
                for index2, k in enumerate(reducer.const):
                    if(j == k):
                        childi.const[index] = clause.const[index2]
            childi = childi.child
    for i in cliste2:
        for index, j in enumerate(i.const):
            for index2, k in enumerate(clause.const):
                if(j == k):
                    i.const[index] = reducer.const[index2]
        childi = i.child
        while childi:
            for index, j in enumerate(childi.const):
                for index2, k in enumerate(clause.const):
                    if(j == k):
                        childi.const[index] = reducer.const[index2]
            childi = childi.child
    return cliste1+cliste2


def theorem_helper(cnf1, cnf2, result):
    finalFlag = False
    for cnf2index, list2 in enumerate(cnf2):
        for i in list2:
            for cnf1index, list1 in enumerate(cnf1):
                for k in list1:
                    if (i.name == k.name and i.negate != k.negate):
                        childFlag = True
                        childi = i.child
                        childk = k.child
                        while childi and childk:
                            if childi.name != childk.name:
                                childFlag = False
                            childi = childi.child
                            childk = childk.child
                        if childFlag:
                            finalFlag = True
                            tmp = clausecreateor(
                                list2) + "$"+clausecreateor(list1)+"$"
                            reduced = clausereducer(list2, i, list1, k)
                            list2.append(i)
                            list1.append(k)
                            if reduced == []:
                                result.append(tmp+"empty")
                                # return [], [], result
                            else:
                                result.append(tmp+clausecreateor(reduced))
                            print(result)
                            # cnf2.pop(cnf2index)
                            # print(cnf1)
                            cnf1.pop(cnf1index)
                            if reduced != []:
                                cnf1.insert(cnf1index, reduced)
                                cnf2.append(reduced)
                            # cnf2.pop(cnf2index)
                            # cnf2.append(list1)
                            return cnf1, cnf2, result
    if finalFlag:
        return cnf1, cnf2, result
    return cnf1, [], result


def theorem_prover(list1, list2):
    # try:
    result = []
    cnf1 = cnfparser(list1)
    cnf2 = cnfparser(list2)
    while cnf2:
        cnf1, cnf2, result = theorem_helper(cnf1, cnf2, result)
        if cnf1 == []:
            return ("yes", result)
    return("no", [])
    # except:
    # return("no", [])


print(theorem_prover(["p(Z)+q(Z)", "~p(Z)+r(Z)", "~q(Z)+r(Z)"], ["~r(Z)"]))
# print(theorem_prover(["animal(Z)", "animal(Y)",
#                       "~animal(x)+loves(John,x)"], ["~loves(John,Y)+~loves(John,Z)"]))
