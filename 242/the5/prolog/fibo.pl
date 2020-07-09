fibo(0, 1).
fibo(1, 1).
fibo(N, R) :-
    N > 0,
    Nmin1 is N - 1,
    Nmin2 is N - 2,
    fibo(Nmin1, R1),
    fibo(Nmin2, R2),
    R is R1 + R2.
