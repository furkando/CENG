module Hw1 where

data Cell = H | P | O | X deriving (Read,Show,Eq)
data Result = Fail | Caught (Int,Int) deriving (Read,Show,Eq)
data Direction = N | S | E | W deriving (Read,Show,Eq)

simulate :: [[Cell]] -> [(Direction, Direction)] -> Result
-- DO NOT CHANGE ABOVE THIS LINE, WRITE YOUR CODE BELOW --
simulate x y = simulator x y 0 (findH x 0) (findP x 0)

simulator x y n h p
    | y == [] = Fail
    | (move h (fst(y!!n)) x) == (move p (snd(y!!n)) x) = Caught (move h (fst(y!!n)) x) 
    | (move h (fst(y!!n)) x) /= (move p (snd(y!!n)) x) = if n+1<(length y) then simulator x y (n+1) (move h (fst(y!!n)) x) (move p (snd(y!!n)) x) else Fail -- (move p (snd(y!!n)) x) 

findHP x = [(findH x 0),(findP x 0)]

findP :: [[Cell]] -> Int -> (Int,Int)   
findP x n
   | P `elem` x!!n = finderP (x!!n) n 0
   | otherwise = findP x (n+1)

finderP :: [Cell] -> Int -> Int -> (Int,Int)    
finderP x n m
    | P == (x!!m) = (m,n)
    | otherwise = finderP x n (m+1)  

findH :: [[Cell]] -> Int -> (Int,Int)   
findH x n
    | H `elem` x!!n = finderH (x!!n) n 0
    | otherwise = findH x (n+1)

finderH :: [Cell] -> Int -> Int -> (Int,Int)    
finderH x n m
    | H == (x!!m) = (m,n)
    | otherwise = finderH x n (m+1)  

findO x n len a
    | O `elem` x!!n  = if n+1<len then findO x (n+1) len (adder (x!!n) n 0 (length (x!!n)) [] ++ a) else adder (x!!n) n 0 (length (x!!n)) [] ++ a
    | O `notElem` x!!n =if n+1<len then findO x (n+1) len a else a

adder x n m len a
    | x!!m == O  = if m+1<len then adder x n (m+1) len ((m,n):a) else (m,n):a
    | x!!m /= O  = if m+1<len then adder x n (m+1) len a else a


move :: (Int,Int) -> Direction -> [[Cell]] -> (Int,Int)
move (x,y) d ar
    | d == E = if (x+1,y) `elem` ((findHP ar)++(findO ar 0 (length ar) [])) then (x+1,y) else (x,y)
    | d == W = if (x-1,y) `elem` ((findHP ar)++(findO ar 0 (length ar) [])) then (x-1,y) else (x,y)
    | d == N = if (x,y-1) `elem` ((findHP ar)++(findO ar 0 (length ar) [])) then (x,y-1) else (x,y)
    | d == S = if (x,y+1) `elem` ((findHP ar)++(findO ar 0 (length ar) [])) then (x,y+1) else (x,y)


