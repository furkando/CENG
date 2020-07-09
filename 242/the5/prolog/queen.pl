/* N-queens problem */

/* pick one element and remainder from the list */
insert(X,R,[X|R]).
insert(X,[A|R],[A|L]) :- insert(X,R,L).

/* check if placement firstarg-secondarg compatible with elements
   in the list */
compatible(_,_,[]).
compatible(K,P,[Kk-Pp|R]) :- P =\= Pp, /* not on same row */
		Kdiff is abs(K-Kk), Pdiff is abs(P-Pp),   /* row col dispos.*/
				Kdiff =\= Pdiff,	/* not on same diag */
		 compatible(K,P,R).	/* rest of the list is compatible */

/* place(Knighlist, Rowlist, CurrentPlacements, Result) */
place([],[],L,L).
place([K|KRest],PList,In,RResult) :- 
	insert(P,PRest,PList), 	/* for all remaining rows and rest */
	compatible(K,P,In),	/* if knight K can be placed on row P */
	place(KRest,PRest,[K-P|In],RResult). /* insert K-P and place rest */


/* five a list of numbers from N to M 
   getMN(1,4,R) gives [1,2,3,4] on R */
getMN(N,N,[N]).
getMN(N,M,[N|R]) :- N<M,Nplus1 is N+1,getMN(Nplus1,M,R).


/* place N queens on NxN board and give result on R */
queen(N,R) :- getMN(1,N,Queens),getMN(1,N,Positions),
		place(Queens,Positions,[],R).
