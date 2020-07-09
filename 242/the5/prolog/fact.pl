fact(0, 1).
fact(N, R) :-
    N > 0,
    Nmin1 is N - 1,
    fact(Nmin1, R2),
    R is N * R2.
