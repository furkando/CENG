f(X, Y) :- Y is 2 * X.
g(X, Y) :- Y is X * X.
func(X, Z) :- f(X, Y), g(Y, Z).
