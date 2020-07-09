sections([sA,sB,sC,sD,sE]) .
adjacent(sA ,sC) .
adjacent(sB ,sC) . 
adjacent(sC ,sD) . 
adjacent(sC ,sE) .

configuration(Compl,Consl,Pl) :- 
    sections(S), 
    length1(S,LengthS),length1(Compl,LengthC),
    (LengthS<LengthC) -> false ; ((Consl = [] ,Compl=[]) -> Pl = [] ; sections(A),configuration2(Compl,Consl,Pl2,A),flatten(Pl2,Pl3),hulo3(Compl,Pl3,Pl)
    ). 


configuration2([],_,[],_):- !.
configuration2(_,_,[],[]):- !.
configuration2([H|T],Consl,[H3|T3],S) :- member(A,S), select(A,S,Rem) , (in_my_list_of_elements(outer_edge(H),Consl) -> hulo(H,H3,A), configuration2(T,Consl,T3,Rem) ; (isClose(H,Consl,Y)-> hulo2(H,Y,H3,A,Sec2),select(Y,T,Remo),select(Sec2,Rem,Rem2), configuration2(Remo,Consl,T3,Rem2) ; putter(H,A,H3), configuration2(T,Consl,T3,Rem) )) .

hulo(Comp,Res,Sec):-  adjnum(Sec,Y) , Y<4 -> putter(Comp,Sec,Res).  

hulo2(Comp1,Comp2,[X,Y],Sec,Sec2):- ado(Sec,Sec2),putter(Comp1,Sec,X),putter(Comp2,Sec2,Y).

length1([], 0).
length1([_|T], R) :-
    length1(T, Y),
    R is Y + 1.

adjnum(X,Y) :-  aggregate_all(count, adjacent(X,_), Count1),aggregate_all(count, adjacent(X,_), Count2) ,Y is (Count1 + Count2) .

in_my_list_of_elements(X,Y) :- 
        member(X, Y).

putter(Comp,Sect,Result) :- Result = put(Comp,Sect). 

ado(S,Res) :- adjacent(S,Res) ; adjacent(Res,S) .

isClose(H,Consl,X) :- in_my_list_of_elements(close_to(H,X),Consl) ; in_my_list_of_elements(close_to(X,H),Consl) .

hulo3([],_,[]):- !.
hulo3([H|T],Res,[H2|T2]):- member(A,Res), A=put(H,_) -> H2=A , hulo3(T,Res,T2). 