edge(a,b).
edge(a,e).
edge(b,d).
edge(b,c).
edge(c,a).
edge(e,b).

tedge(Node1,Node2) :-
    edge(Node1,SomeNode),
    edge(SomeNode,Node2).

path(Node1,Node2) :-
    edge(Node1,Node2).
path(Node1,Node2) :-
    edge(Node1,SomeNode),
    path(SomeNode,Node2).

pathLength(Node1,Node2,1) :-
    edge(Node1,Node2).
pathLength(Node1,Node2,X) :-
    edge(Node1,SomeNode),
    pathLength(SomeNode,Node2,Y),
    X is Y + 1.
