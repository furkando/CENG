/* pick an arbitrary element out of list and the remaining list */
/* insert(X,Y,[1,2,3])  instantiate 1,[2,3] ; 2,[1,3] ; 3,[2,2] on X,Y */
/* or insert El in list Remlist on every possible position to have 3rd arg */
/* insert(1,[2,3],X) instantiate [1,2,3], [2,1,3], [2,3,1] on X */
insert(El,RemList,[El|RemList]).
insert(El,[A|RemList],[A|R]) :- insert(El,RemList,R).

/* permut(List,R,N) finds all N permutations of the List in R */
/* permut([1,2,3],R,2) finds [1,2],[2,1],[1,3],[3,1],[2,3],[3,2] on R */
permut(_,[],0).		/* 0 permutation of anything is empty list */
/* pick each element and its remainder, find all N-1 permutations of */
/* remainder, insert to the begining */
permut(List,[El|RestPerm],N) :- N>0,M is N-1,
		insert(El,Rest,List),permut(Rest,RestPerm,M).

/* combin(List,R,N) finds all N combinations of the List in R */
/* combin([1,2,3],R,2) finds [1,2],[1,3],[2,3] on R */
combin(_,[],0).		/* 0 combination of anything is empty list */
/* choice to include first element in result. remove it find all N-1  */
/* combinations, then insert to the beginning */
combin([H|Rem],[H|RemComb],N):- N>0,M is N-1, combin(Rem,RemComb,M).
/* choice not to include first element in result. Just remove it */
/* and find all N combinations on the remainder */
combin([_|Rem],Res,N):- N>0, combin(Rem,Res,N).


nmember(X,[X|_]).
nmember(X,[_|R]) :- nmember(X,R).


/* append implementation */
mappend([], X, X).
mappend([X|R],T,[X|RandT]) :- mappend(R,T,RandT).

/* reverse implementation, slow version */
reverseslow([],[]).	/* reverse of empty list is empty */
/* get first element, reverse remainder, append first at the end */
reverseslow([X|R],Reversed) :- reverseslow(R,RevR), mappend(RevR,[X],Reversed).


/* reverse implementation, fast version */
/* reverse2(Input,Stack,Result) */
reverse2([],X,X).	/* if input is empty, set result to stack */
reverse2([X|R],Y,T) :-  /* push first element on stack and recurse */
	reverse2(R,[X|Y],T).

reversefast(X,Y) :- reverse2(X,[],Y).   /* search reverse2 with empty stack */
