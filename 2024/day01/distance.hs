module Day1 where
import Data.List

main :: IO ()
main = do
    input <- getLine
    case input of
        "1" -> part1
        "2" -> part2
        _ -> print "unknown action. try again\n" >> main

part1 :: IO ()
part1 = do
    input <- readFile "input"
    let l1 = sort $ read <$> (takeEven $ words input)
        l2 = sort $ read <$> (takeOdd $ words input)
        in print $ distance 0 l1 l2

part2 :: IO ()
part2 = do
    input <- readFile "input"
    let l1 = sort $ read <$> (takeEven $ words input)
        l2 = sort $ read <$> (takeOdd $ words input)
        in print $ similarity 0 l1 l2

similarity :: Integer -> [Integer] -> [Integer] -> Integer
similarity n [] _ = n
similarity n (x:xs) ys =
    let count = fromIntegral (length $ filter (==x) ys)
        in similarity (n + count * x) xs ys

distance :: Integer -> [Integer] -> [Integer] -> Integer
distance n [] [] = n
distance n (x:xs) (y:ys) =
    let d = abs $ x - y
        in distance (n + d) xs ys

takeEven :: [a] -> [a]
takeEven [] = []
takeEven (x:xs) = x : (takeEven $ drop 1 xs)

takeOdd :: [a] -> [a]
takeOdd [] = []
takeOdd (x:xs) = takeEven xs