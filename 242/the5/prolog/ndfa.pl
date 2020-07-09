/* ndfa processing of decimal, octal and hexadecimal numbers in C style*/
odigit(0).
odigit(1).
odigit(2).
odigit(3).
odigit(4).
odigit(5).
odigit(6).
odigit(7).

digit(8).
digit(9).
digit(X) :- odigit(X).

hexdigit(a).
hexdigit(b).
hexdigit(c).
hexdigit(d).
hexdigit(e).
hexdigit(f).
hexdigit(X) :- digit(X).



% 0 can start octal or hexedecimal. 0 itself decimal
transition(start,0,sZero).
transition(start,0,sOctal).
transition(start,0,sHex).
% any other decimal digit makes it decimal
transition(start,X,sDecimalFin) :- digit(X), X =\= 0.
% on octal state octal digits continue accepting as octal
transition(sOctal,X,sOctalFin) :- odigit(X).
transition(sOctalFin,X,sOctalFin) :- odigit(X).
% x following 0 makes it hexadecimal
transition(sHex,x,sHex2).
% hexadecimal digits continue accepting hex number
transition(sHex2,X,sHexFin) :- hexdigit(X).
transition(sHexFin,X,sHexFin) :- hexdigit(X).
% decimal number continue accepting decimal
transition(sDecimalFin,X,sDecimalFin) :- digit(X).
% the final states with their categories
final(sZero,decimal).
final(sOctalFin,octal).
final(sHexFin,hexadecimal).
final(sDecimalFin,decimal).

% no empty transition in this ndfa, added to stop compiler complaining
empty(snonexist,snonexist).



% if there is a transition from S1 to S2 with X, take it, parse the rest
parse(S1,[X|T],R) :- transition(S1,X,S2), parse(S2,T,R).
% if there is an empty transition from S1 to S2, parse from S2
parse(S1,T,R) :- empty(S1,S2), parse(S2,T,R).
% if input is empty and S1 is final state, accept it
parse(S1,[],R) :- final(S1,R).

% query as: accept([0,x,1,2,e,f],R). 
accept(L,R) :- parse(start,L,R).

