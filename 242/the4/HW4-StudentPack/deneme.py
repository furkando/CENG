import sys,re
from sympy.solvers import solve
from sympy import Symbol
from fractions import gcd
from collections import defaultdict

Ls=list('abcdefghijklmnopqrstuvwxyz')
eq=sys.argv[1]
Ss,Os,Es,a,i=defaultdict(list),Ls[:],[],1,1
for p in eq.split('->'):
    for k in p.split('+'):
        c = [Ls.pop(0), 1]
        for e,m in re.findall('([A-Z][a-z]?)([0-9]*)',k):
            m=1 if m=='' else int(m)
            a*=m
            d=[c[0],c[1]*m*i]
            Ss[e][:0],Es[:0]=[d],[[e,d]]
    i=-1
Ys=dict((s,eval('Symbol("'+s+'")')) for s in Os if s not in Ls)
Qs=[eval('+'.join('%d*%s'%(c[1],c[0]) for c in Ss[s]),{},Ys) for s in Ss]+[Ys['a']-a]
k=solve(Qs,*Ys)
if k:
    N=[k[Ys[s]] for s in sorted(Ys)]
    g=N[0]
    for a1, a2 in zip(N[0::2],N[1::2]):g=gcd(g,a2)
    N=[i/g for i in N]
    pM=lambda c: str(c) if c!=1 else ''
    print '->'.join('+'.join(pM(N.pop(0))+str(t) for t in p.split('+')) for p in eq.split('->'))
else:print 'Nope!'