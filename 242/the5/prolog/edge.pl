edge(a,b).
edge(a,e).
edge(b,d).
edge(b,c).
edge(c,a).
edge(e,b).

tedge(Node1, Node2) :- edge(Node1, X), edge(X, Node2).

path(Node1, Node2) :- edge(Node1, Node2).
path(Node1, Node2) :- edge(Node1, X), path(X, Node2).

pathlen(Node1, Node2, 1) :- edge(Node1, Node2).
pathlen(Node1, Node2, N) :- edge(Node1, X), pathlen(X, Node2, Y), N is Y + 1.

pathLenExact(Node1, Node2, 1) :- edge(Node1, Node2).
pathLenExact(Node1, Node2, Cnt) :- 
    Cnt > 1,
    Value is Cnt - 1, 
    edge(Node1, SomeNode),
    pathLenExact(SomeNode, Node2, Value).


pathTrace(X, Y, [Y]) :- edge(X, Y).
pathTrace(X, Y, [Z | T]) :- edge(X, Z), pathTrace(Z, Y, T).
