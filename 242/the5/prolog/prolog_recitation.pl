% METU Department of Computer Engineering
% Spring 2017-18 CENG242 Recitation 2 - Prolog


% DATA TYPES
% Atoms : mary, food, likes etc.
% Numbers : 0, 1, etc.
% Variables : X, Y, etc.
% Structures : hates(mary,food)
hates(mary,food).
father(charles, james).
add_3_and_double(X,Y) :- Y is (X+3)*2.


% FACTS
apple.
car(bmw).
female(mary).
eats(mary, icecream).


% RULES
toy(doll).
toy(train).
plays(ann,train).
likes(ann, X) :- toy(X), plays(ann, X). % ann likes X if X is a toy and ann plays with X.
likes(john, Y) :- likes(ann, Y).	% john likes Y if ann likes Y.

parent(pam, bob).
parent(tom, bob).
parent(tom, liz).
parent(bob, ann).
parent(bob, pat).
parent(pat, jim).
predecessor(X, Y) :- parent(X, Y).
predecessor(X, Z) :- parent(X, Y), predecessor(Y, Z).
% Order of clauses and goals is important. The following predicate definition results in infinite recursion. (local stack overflow)
% predecessor(X, Z) :- predecessor(Y, Z), parent(X, Y).
% predecessor(X, Y) :- parent(X, Y).


% LISTS
size([],0). % Acts as the base case in the recursion.
size([_|T],N) :- size(T,N1), N is N1+1.
% Note that size([H|T], X) :- N is N1+1, size(T,N1). causes instantiation error while evaluating 'is' operator. 'is' expects the right hand side to be instantiated at the time of evaluation (i.e. value of N1 must be known).


% OPERATORS
b.
c.
a :- b,!. % a if b

a :- b,c. % a if b and c.
a :- b;c. % a if b or c.
a :- not(b). % a if b fails
a :- b -> c;d. % a if (if b then c else d)

student(marry).
person(X) :- student(X).

animal(monkey).
animal(hawk).
flies(hawk).
bird(X) :- animal(X), flies(X).

dead(michaeljackson).
alive(X) :- not(dead(X)).

min(A, B, Min) :- A < B -> Min = A ; Min = B.


% CUTS
% if p holds then r implies g, and if ¬p holds then t implies g.
p.
r.
g :- p,!,r.
g :- t.

% Don’t try other choices of red and color if X satisfies red
red(car).
red(bird).
red(pen).
color(X,red) :- red(X), !.

% In the presence of backtracking, incorrect answers can result.
% max(A,B,B) :- A < B.
% max(A,B,A).

% To prevent backtracking to the second rule the cut symbol is inserted into the first rule.
max(A,B,B) :- A < B,!.
max(A,B,A).