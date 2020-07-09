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

chain1(Node1, Node2, C) :-
    edge(Node1, Node2),
    C = [Node1, Node2].

chain1(Node1, Node2, C) :-
    edge(Node1, SomeNode),
    path(SomeNode, Node2),
    C = [Node1 | T],
    chain1(SomeNode, Node2, T).

chain(X, Y, [X, Y]) :- 
    edge(X, Y).

chain(X, Y, [X | T]) :-
    edge(X, Z),
    path(Z, Y),
    chain(Z, Y, T).
chain(X, Y, []).


append([], Y, Y).
append([X | T], Y, [X | R]) :-
    append(T, Y, R).

myrev([], []).
myrev([X | T], R) :-
    myrev(T, K),
    append(K, [X], R).

mem(X, [X | _]) :- !.
mem(X, [_ | T]) :-
    mem(X, T).

a(X) :- b(X), !, c(X).
a(X) :- d(X).

b(1).
c(3).
d(4).


isPalin([]).
isPalin(L) :-
    myrev(L, L).
