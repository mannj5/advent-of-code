module Disk where

import Data.Char (digitToInt)

{-- total number of files in
    d9test = 10 (0 - 9)
    d9input = 10000 (0 - 9999)

    total number of 'fileID' numbers in
    d9test = 28
    d9input = 50040
--}

main :: IO ()
main = do
    input <- readFile "input"
    part1 input
    part2 input

part1 input =
    let expanded = expand input
        compacted = compact expanded 50040 -- number of Just's in the expanded form
        total = checksum compacted
    in print total

-- if block is file: File id length
data Block = File Int Int | Free Int deriving Show

part2 input =
    let expanded = expand2 input
        filesOnly = getFiles expanded
        compacted = compact2 expanded filesOnly
        total = checksum2 compacted
    in print total

getFiles :: [Block] -> [Block]
getFiles disk = go disk []
    where
        go [] acc = acc
        go (b:bl) acc = case b of
            Free _ -> go bl acc
            File id l -> go bl ((File id l):acc)

checksum2 :: [Block] -> Int
checksum2 disk = go disk 0 0
    where
        go [] _ total = total
        go (x:xs) n total = case x of
            Free l -> go xs (n + l) total
            File id l -> go xs (n + l) (total + hsum (File id l) n)

hsum :: Block -> Int -> Int
hsum file n = go file n 0
    where
        go (File id 0) n total = total
        go (File id l) n total = go (File id (l - 1)) (n + 1) (total + (n * id))

compact2 :: [Block] -> [Block] -> [Block]
compact2 disk [] = disk
compact2 disk (file:files) = if freespace disk file
    then compact2 (go disk file) files
    else compact2 disk files
    where
        go [] _ = []
        go (x:xs) bl = case bl of
            File id l -> case x of
                File id' l' -> x:(go xs (File id l))
                Free n -> if n >= l
                    then if n - l == 0
                        then (File id l):(go xs (Free id))
                        else (File id l):(Free (n - l)):(go xs (Free id))
                    else x:(go xs (File id l))
            Free id -> case x of
                File id' l' -> if id == id'
                    then (Free l'):xs
                    else x:(go xs (Free id))
                _ -> x:(go xs (Free id))

freespace :: [Block] -> Block -> Bool
freespace [] _ = False
freespace (d:disk) (File id l) = case d of
    File id' l' -> if id == id'
        then False
        else freespace disk (File id l)
    Free n -> if n >= l
        then True
        else freespace disk (File id l)

expand2 :: String -> [Block]
expand2 disk = go disk 0 True
    where
        go [] _ _ = []
        go (x:xs) n True = (File n (digitToInt x)):(go xs n False)
        go (x:xs) n False = (Free (digitToInt x)):(go xs (n + 1) True)

expand :: String -> [Maybe Int]
expand disk = go disk 0 True
    where
        go [] _ _ = []
        go (x:xs) n True = put (Just n) (digitToInt x) (go xs n False)
        go (x:xs) n False = put Nothing (digitToInt x) (go xs (n + 1) True)

put :: a -> Int -> [a] -> [a]
put c 0 xs = xs
put c n xs = put c (n - 1) (c:xs)

compact :: [Maybe Int] -> Int -> [Int]
compact disk limit = go disk (reverse disk) limit
    where
        go _ _ 0 = []
        go (x:xs) (y:ys) n = case x of
            Just l -> l:(go xs (y:ys) (n - 1))
            Nothing -> case y of
                Just r -> r:(go xs ys (n - 1))
                Nothing -> go (x:xs) ys n

checksum :: [Int] -> Int
checksum ns = go (zip [0..] ns) 0
    where
        go [] total = total
        go ((ix, n):xs) total = go xs (total + (ix * n))


-- Helper functions for determining magic values that make my life easier :)
getJustTotal xs = length [() | Just _ <- xs]

getTotalSize xs = sum $ snd <$> filter (even . fst) (zip [0..] xs)