from random import randint
from random import shuffle
import sys


starting_ammo = randint(50,200)
vertice_count = int(sys.argv[1])
key_vertice = randint(2,vertice_count)
lab_vertice = randint(2,vertice_count)
chamber_vertice = randint(2,vertice_count)
edges = []
ammos = []
print("vertice count %d" % vertice_count)
lis = list( range(1, vertice_count+1) )
shuffle(lis)
for v in lis:
	for u in range(1, vertice_count+1):
		r = randint(5,1000)
		if r<105:
			edges.append( (u,v, r ) )
print("done edges")
shuffle(lis)
for i in lis:
	if randint(1,20) == 1:
		ammos.append( (i,randint(100,1000)) )
		if len(ammos)==2:
			break

print("ammos: ",len(ammos))

odd_locks = []
even_locks = []

for i in range(1,vertice_count+1):
	if randint(1,20) == 1:
		odd_locks.append( i )
	elif randint(1,20) == 1:
		even_locks.append( i )


def w(a):
	if type(a)==int or type(a)==str:
		return str(a)+" \n"
	elif type(a) == tuple or type(a) == list:
		a = map(str,a)
		return " ".join(a) + " \n"


with open("inp"+str(sys.argv[2])+".txt", "w") as f:
	f.write(w( starting_ammo ))
	f.write(w( vertice_count ))
	f.write(w( chamber_vertice ))
	f.write(w( key_vertice ))
	f.write(w( lab_vertice ))

	f.write(w( len(odd_locks) ))
	for l in odd_locks:
		f.write(w( l ))

	f.write(w( len(even_locks) ))
	for l in even_locks:
		f.write(w( l ))

	f.write(w( len(edges) ))
	for e in edges:
		
		f.write(w( e ))

	f.write(w( len(ammos) ))
	for a in ammos:
		f.write(w( a ))



















