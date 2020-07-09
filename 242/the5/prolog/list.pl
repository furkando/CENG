append1([], L, L).
append1([X|Y], L, R) :-
    R = [X|Z],
    append1(Y, L, Z).

append2([], L, L).
append2([H|T], L, [H|R]) :-
    append(T, L, R).

reverse1([], []).
reverse1([H|T], RevList) :-
    reverse1(T, TailReverse),
    append(TailReverse, [H], RevList).

length1([], 0).
length1([_|T], R) :-
    length1(T, Y),
    R is Y + 1.

isPalin([]).
isPalin(L) :-
    reverse1(L, L).

posAt([X | _], 0, X) :- !.
posAt([_ | T], N, Y) :-
    M is N - 1,
    posAt(T, M, Y).

prefixOf([], _).
prefixOf([X | T1], [X | T2]) :-
    prefixOf(T1, T2).

sublist([], _).
sublist(L1, L2) :- prefixOf(L1, L2).
sublist(L1, [_ | T]) :- sublist(L1, T).
