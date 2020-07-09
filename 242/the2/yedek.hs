module Hw2 where

import Data.List -- YOU MAY USE THIS MODULE FOR SORTING THE AGENTS

data Level = Newbie | Intermediate | Expert deriving (Enum, Eq, Ord, Show, Read)
data Hunter = Hunter {hID::Int, hlevel::Level, hEnergy::Int, hNumberOfCatches::Int, hActions::[Direction]} deriving (Eq, Show, Read)
data Prey = Prey {pID::Int, pEnergy::Int, pActions::[Direction]} deriving (Eq, Show, Read)
data Cell = O | X | H Hunter | P Prey | T deriving (Eq, Show, Read)
data Direction = N | S | E | W deriving (Eq, Show, Read)
type Coordinate = (Int, Int)
-- DO NOT CHANGE THE DEFINITIONS ABOVE. --


-- INSTANCES OF Ord FOR SORTING, UNCOMMENT AND COMPLETE THE IMPLEMENTATIONS --
-- instance Ord Hunter where
--    compare

-- instance Ord Prey where
--    compare


-- WRITE THE REST OF YOUR CODE HERE --
findP x n m = if n+1 < (length x) then findP x (n+1) (finderP (x!!n) n 0 [] ++ m ) else finderP (x!!n) n 0 [] ++ m

finderP x n m a
    | isPrey(x!!m) == True = if m+1 < (length x) then finderP x n (m+1) (((x!!m),(m,n)):a) else ((x!!m),(m,n)):a
    | isPrey(x!!m) == False = if m+1 < (length x) then finderP x n (m+1) a else a 

findH x n m = if n+1 < (length x) then findH x (n+1) (finderH (x!!n) n 0 [] ++ m ) else finderH (x!!n) n 0 [] ++ m

finderH x n m a
    | isHunter(x!!m) == True = if m+1 < (length x) then finderH x n (m+1) (((x!!m),(m,n)):a) else ((x!!m),(m,n)):a
    | isHunter(x!!m) == False = if m+1 < (length x) then finderH x n (m+1) a else a 

findO x n a
    | O `elem` x!!n  = if n+1<(length x) then findO x (n+1) (finderO (x!!n) n 0 [] ++ a) else finderO (x!!n) n 0 [] ++ a
    | O `notElem` x!!n =if n+1<(length x) then findO x (n+1) a else a

finderO x n m a
    | x!!m == O  = if m+1<(length x) then finderO x n (m+1) ((m,n):a) else (m,n):a
    | x!!m /= O  = if m+1<(length x) then finderO x n (m+1) a else a

findT x n a
    | T `elem` x!!n  = if n+1<(length x) then findT x (n+1) (finderT (x!!n) n 0 [] ++ a) else finderT (x!!n) n 0 [] ++ a
    | T `notElem` x!!n = if n+1<(length x) then findT x (n+1) a else a

finderT x n m a
    | x!!m == T  = if m+1<(length x) then finderT x n (m+1) ((m,n):a) else (m,n):a
    | x!!m /= T  = if m+1<(length x) then finderT x n (m+1) a else a

adder x a = a ++ (findO x 0 []) ++ boomBangs (findP x 0 []) ++ boomBangs (findH x 0 []) ++ (findT x 0 [])

preys x a = a ++  boomBangs (findP x 0 []) 

hunters x a = a ++  boomBangs (findH x 0 []) 

hp x = reverse ((findH x 0 [])) ++ reverse ((findP x 0 []))

rfindP x = reverse (findP x 0 [])

rfindH x = reverse (findH x 0 [])

boomBangs xs = [ snd (xs!!x) | x <- [0 .. (length xs)-1]]  

preyer :: Cell -> Prey
preyer (P b) = b 

hunterer :: Cell -> Hunter
hunterer (H b) = b 

preyo :: Prey -> Cell
preyo b = (P b)

huntero :: Hunter -> Cell
huntero b = (H b)

isPrey :: Cell -> Bool
isPrey (P _) = True
isPrey _     = False


isHunter :: Cell -> Bool
isHunter (H _) = True
isHunter _     = False


instance Ord Hunter where 
    compare (Hunter id1 level1 energy1 catch1 acts1) (Hunter id2 level2 energy2 catch2 acts2) 
        | level1 > level2 = GT
        | level1 == level2 = if energy1 > energy2 then GT else if energy1 == energy2 then (if catch1 > catch2 then GT else if catch1 == catch2 then (if id1 < id2 then GT else LT) else LT) else LT
        | level1 < level2 = LT

instance Ord Prey where
    compare (Prey id1 energy1 acts1) (Prey id2 energy2 acts2)
        | energy1 > energy2 = GT
        | energy1 == energy2 = if id1 < id2 then GT else LT
        | energy1 < energy2 = LT

moveH (q,(x,y)) d ar n
    | d == E = if (x+1,y) `elem` (adder ar []) then (if hEnergy (hunterer q) < 10 then ((incH (hunterer(q))),(x,y)) else ((decH (hunterer(q))),(x+1,y))) else ((decH (hunterer(q))),(x,y))
    | d == W = if (x-1,y) `elem` (adder ar []) then (if hEnergy (hunterer q) < 10 then ((incH (hunterer(q))),(x,y)) else ((decH (hunterer(q))),(x-1,y))) else ((decH (hunterer(q))),(x,y))
    | d == N = if (x,y-1) `elem` (adder ar []) then (if hEnergy (hunterer q) < 10 then ((incH (hunterer(q))),(x,y)) else ((decH (hunterer(q))),(x,y-1))) else ((decH (hunterer(q))),(x,y))
    | d == S = if (x,y+1) `elem` (adder ar []) then (if hEnergy (hunterer q) < 10 then ((incH (hunterer(q))),(x,y)) else ((decH (hunterer(q))),(x,y+1))) else ((decH (hunterer(q))),(x,y))

moveP (q,(x,y)) d ar n
    | d == E = if (x+1,y) `elem` (adder ar []) then (if pEnergy (preyer q) < 10 then (if (x,y) `elem` (findT ar 0 []) then ((trapP2 (preyer(q))),(x,y)) else ((incP (preyer(q))),(x,y)))  else (if (x+1,y) `elem` (findT ar 0 []) then ((trapP (preyer(q))),(x+1,y)) else ((decP (preyer(q))),(x+1,y)))) else (if (x,y) `elem` (findT ar 0 []) then ((trapP (preyer(q))),(x,y)) else ((decP (preyer(q))),(x,y)))
    | d == W = if (x-1,y) `elem` (adder ar []) then (if pEnergy (preyer q) < 10 then (if (x,y) `elem` (findT ar 0 []) then ((trapP2 (preyer(q))),(x,y)) else ((incP (preyer(q))),(x,y)))  else (if (x-1,y) `elem` (findT ar 0 []) then ((trapP (preyer(q))),(x-1,y)) else ((decP (preyer(q))),(x-1,y)))) else (if (x,y) `elem` (findT ar 0 []) then ((trapP (preyer(q))),(x,y)) else ((decP (preyer(q))),(x,y)))
    | d == N = if (x,y-1) `elem` (adder ar []) then (if pEnergy (preyer q) < 10 then (if (x,y) `elem` (findT ar 0 []) then ((trapP2 (preyer(q))),(x,y)) else ((incP (preyer(q))),(x,y)))  else (if (x,y-1) `elem` (findT ar 0 []) then ((trapP (preyer(q))),(x,y-1)) else ((decP (preyer(q))),(x,y-1)))) else (if (x,y) `elem` (findT ar 0 []) then ((trapP (preyer(q))),(x,y)) else ((decP (preyer(q))),(x,y)))
    | d == S = if (x,y+1) `elem` (adder ar []) then (if pEnergy (preyer q) < 10 then (if (x,y) `elem` (findT ar 0 []) then ((trapP2 (preyer(q))),(x,y)) else ((incP (preyer(q))),(x,y)))  else (if (x,y+1) `elem` (findT ar 0 []) then ((trapP (preyer(q))),(x,y+1)) else ((decP (preyer(q))),(x,y+1)))) else (if (x,y) `elem` (findT ar 0 []) then ((trapP (preyer(q))),(x,y)) else ((decP (preyer(q))),(x,y)))

lenghter x = length (pActions (preyer (fst ((findP x 0 [])!!0))))

simulate x = ([(hunterer (fst ((simulator x (hp x) 0)!!a)),snd ((simulator x (hp x) 0)!!a)) | a<- [0..(length (hunters x []))-1] ] , [ (preyer (fst ((simulator x (hp x) 0)!!b)),snd ((simulator x (hp x) 0)!!b)) | b<- [(length (hunters x []))..(length (hp x))-1] , getPID (preyer (fst ((simulator x (hp x) 0)!!b))) /= -1 ])

simulator x hq n  
    | n < lenghter x && numberOfP x hq 0 (length (hunters x [])) > 0 =  simulator x (eater (changer ([moveH (fst (hq!!a),snd (hq!!a)) ((hActions (hunterer (fst (((hq)!!a))))!!0)) x n | a <- [0..(length (hunters x []))-1]] ++ [moveP ( (fst (((hq)!!(a)))),snd (((hq)!!(a)))) ((pActions (preyer (fst ((hq)!!(a)))))!!0) x n | a <- [(length (hunters x []))..(length (hp x))-1]])) [] 0 x) (n+1)
    | otherwise = hq

changer hq = changerhunter (sort [(hunterer(fst (hq!!n)),snd (hq!!n)) | n<-[0..(length hq)-1] , isHunter (fst (hq!!n)) == True ]) ++ changerprey (sort [(preyer(fst (hq!!n)),snd (hq!!n)) | n<-[0..(length hq)-1] , isPrey (fst (hq!!n)) == True ]) 

changerhunter hq = reverse [(huntero(fst (hq!!n)),snd (hq!!n)) | n<-[0..(length hq)-1] ]

changerprey hq = [(preyo(fst (hq!!n)),snd (hq!!n)) | n<-[0..(length hq)-1] ] 

numberOfP x hq n m
    | m < length (hp x) = if getPID(preyer (fst (hq!!m))) /= -1 then numberOfP x hq (n+1) (m+1) else numberOfP x hq n (m+1)
    | otherwise = n

changer2 hq = changerhunter2 (sort [(hunterer(fst (hq!!n)),snd (hq!!n)) | n<-[0..(length hq)-1] , isHunter (fst (hq!!n)) == True ]) ++ changerprey (sort [(preyer(fst (hq!!n)),snd (hq!!n)) | n<-[0..(length hq)-1] , isPrey (fst (hq!!n)) == True ]) 

changerhunter2 hq =  [(huntero(fst (hq!!n)),snd (hq!!n)) | n<-[0..(length hq)-1] ]

eater hq bos n x
    | n < (length (hunters x [])) = eater hq (bos ++ (eaterp bos hq (hq!!n) n (length (hunters x [])) x)) (n+1) x
    | n < (length (hp x)) = eater hq (bos ++ (eaterh bos hq (hq!!n) 0 (length (hunters x [])) x)) (n+1) x
    | otherwise  = bos 

eaterhelp hq bos n x
    | n < (length (hunters x [])) = eaterhelp hq (bos ++ (eaterp bos hq (hq!!n) n (length (hunters x [])) x)) (n+1) x
    | otherwise  = bos 

eaterp bos hq n k m x 
    | m < (length (hp x)) = if (snd n)==(snd (hq!!m)) && getPID (preyer (fst ((eaterh (bos) (hq) (hq!!m) 0 (k) x)!!0))) /= -1 && getPID (preyer (fst (hq!!m))) /= -1 && (getcatch (hunterer (fst n))-getcatch (hunterer (fst (hq!!k)))) < 2 then eaterp bos hq (catchH (hunterer (fst n)),snd n) k (m+1) x else eaterp bos hq n k (m+1) x
    | otherwise = [n]

eaterh pre hq n m k x
    | m < k = if (snd n)==(snd (hq!!m)) &&  (getcatch (hunterer (fst (pre!!m))) - getcatch (hunterer (fst (hq!!m)))) > 0  then [(catchP (preyer (fst n)),snd n)] else eaterh pre hq n (m+1) k x
    | otherwise = [n]

decP (Prey id1 energy1 acts1) = P (Prey id1 (energy1-1) (tail acts1))

decH (Hunter id1 level1 energy1 catch1 acts1) = H (Hunter id1 level1 (energy1-1) catch1 (tail acts1))

incH (Hunter id1 level1 energy1 catch1 acts1) = H (Hunter id1 level1 (energy1+1) catch1 (tail acts1))

incP (Prey id1 energy1 acts1) = P (Prey id1 (energy1+1) (tail acts1))

trapP (Prey id1 energy1 acts1) = if energy1<11 then P (Prey id1 (0) (tail acts1)) else P (Prey id1 (energy1-11) (tail acts1))

trapP2 (Prey id1 energy1 acts1) = if energy1<9 then P (Prey id1 (0) (tail acts1)) else P (Prey id1 (energy1-9) (tail acts1))

catchH (Hunter id1 level1 energy1 catch1 acts1) =if energy1>80 then H (Hunter id1 level1 (100) (catch1+1) acts1) else H (Hunter id1 level1 (energy1+20) (catch1+1) acts1)

catchP (Prey id1 energy1 acts1) = P (Prey (-1) energy1 acts1)

getPID (Prey id1 energy1 acts1) = id1

getcatch (Hunter id1 level1 energy1 catch1 acts1) =  catch1 

-- simulatore x hq n
--     | n < lenghter x =  (simulatore x (hq ++ [moveH (fst (hq!!(a+n*length (hp x))),snd (hq!!(a+n*length (hp x)))) ((hActions (hunterer (fst (((hq)!!(a+n*length (hp x))))))!!0)) x n | a <- [0..(length (hunters x []))-1]] ++ [moveP ( (fst (((hq)!!((a+n*length (hp x)))))),snd (((hq)!!((a+n*length (hp x)))))) ((pActions (preyer (fst ((hq)!!((a+n*length (hp x)))))))!!0) x n | a <- [(length (hunters x []))..(length (hp x))-1]]) (n+1)) 
--     | otherwise =  boomBangs hq

-- arroh ad a n b
--     | n <= lenghter ad = arroh ad a (n+1) (b++[[a!!(x+n*(length (rfindH ad))) | x <- [0 .. ((length (rfindH ad)) - 1)]]])
--     | otherwise = b

-- arrop ad a n b
--     | n <= lenghter ad = arrop ad a (n+1) (b++[[a!!(x+n*(length (rfindP ad))) | x <- [0 .. ((length (rfindP ad)) - 1)]]])
--     | otherwise = b

-- simulatorep x hq n
--     | n < lenghter x =  simulatorep x (hq ++ [moveP ( (fst (hq!!( a+n * length (rfindP x)))),snd (((hq)!!((a+n * length (rfindP x)))))) ((pActions (preyer (fst (hq!!(a+n*length (rfindP x))))))!!0) x n | a <- [0..(length (rfindP x))-1]]) (n+1)
--     | otherwise =  boomBangs hq

-- simulatoreh x hq n
--     | n < lenghter x =  simulatoreh x (hq ++ [moveH (fst (hq!!(a+n*length (rfindH x))),snd (hq!!(a+n*length (rfindH x)))) ((hActions (hunterer (fst (((hq)!!(a+n*length (rfindH x))))))!!0)) x n | a <- [0..(length (hunters x []))-1]]) (n+1)
--     | otherwise =  boomBangs hq
