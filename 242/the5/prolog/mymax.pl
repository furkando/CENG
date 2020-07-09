mymax(X,Y,X) :- X >= Y, !.
mymax(X,Y,Y) :- Y > X.
