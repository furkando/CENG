#path = [1,163,218,409,115,224,164,32,454,303,48,174,497,174,382,131]
path = [1,163,218,409, 272, 174, 497, 174, 48, 303, 48, 47, 224, 381, 365, 131 ]

def weight(edges,u,v):
	w1 = 99999
	w2 = 99999
	for e in edges:
		if e[0] == u and e[1] == v:
			w1 = e[2]
		elif e[0] == v and e[1] == u:
			w2 = e[2]

	return min(w1,w2)


edges = []

with open("output.txt","r") as f:
	a = f.readline()
	while a!="":
		a = a.strip().split()
		a = list(map(int,a))
		edges.append(a)
		a = f.readline()

#print(edges)
#print(len(edges))
total_cost = 0
for i in range(len(path)-1):
	total_cost += weight(edges,path[i],path[i+1])

print(total_cost)

print( 140 - total_cost + 166 + 829)
