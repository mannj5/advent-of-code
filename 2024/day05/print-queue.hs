module Day5 where

type Dict = [(Int, [Int])]
type Entry = (Int, [Int])

-- First Section: decide which part to solve

main :: Int -> IO ()
main n = do
    input <- readFile "input"
    let parts = splitInput input
    let dict = foldr add [] (fst parts)
    let queue = snd parts
    mode n queue dict

mode :: Int -> [[Int]] -> Dict -> IO ()
mode 1 q d = print $ addvalid q d
mode 2 q d = print $ foldr ((+) . middle) 0 $ fix q d

-- Second Section: split the input into sections and compute dictionary

splitInput :: String -> ([(Int, Int)], [[Int]])
splitInput input = (first xs, second xs)
    where
        xs = words input

first :: [String] -> [(Int, Int)]
first (x:xs) = case splitOn '|' x of
    [c1, c2] -> (read c1, read c2):(first xs)
    _ -> []

second :: [String] -> [[Int]]
second [] = []
second (x:xs) = case splitOn ',' x of
    [one] -> second xs
    (y:ys) -> (map read (y:ys)):(second xs)

splitOn :: Char -> String -> [String]
splitOn c xs = case break (== c) xs of
    (prefix, "") -> [prefix]
    (prefix, _:rest) -> prefix : (splitOn c rest)

add :: (Int, Int) -> Dict -> Dict
add (key, val) dict = case lookup key dict of
    Nothing -> (key, [val]):dict
    Just _ -> insert key val <$> dict

insert :: Int -> Int -> Entry -> Entry
insert key val (k, vs) = if key == k then (k, val:vs) else (k, vs)

-- Third Section: helper functions

addvalid :: [[Int]] -> Dict -> Int
addvalid xs d = foldr ((+) . (f d)) 0 xs
    where f d x = if isvalid x d then middle x else 0

fix :: [[Int]] -> Dict -> [[Int]]
fix q d = map (bubbleSort d) (filter f q)
    where f x = not $ isvalid x d

bubbleSort :: Dict -> [Int] -> [Int]
bubbleSort d q 
    | q == next = q
    | otherwise = bubbleSort d next
    where next = bubble d q

bubble :: Dict -> [Int] -> [Int]
bubble d [x] = [x]
bubble d (x:y:z) = if has x y d then x : (bubble d (y:z)) else bubble d (y:x:z)

-- only works for odd length lists
middle :: [a] -> a
middle xs = go xs xs
    where
        go (_:_:x) (_:y) = go x y
        go [_] (x:_) = x 

isvalid :: [Int] -> Dict -> Bool
isvalid [x] d = True
isvalid (x:y:z) d = if has x y d then isvalid (y:z) d else False

has :: Int -> Int -> Dict -> Bool
has x y d = case lookup x d of
    Nothing -> False
    Just lst -> elem y lst
