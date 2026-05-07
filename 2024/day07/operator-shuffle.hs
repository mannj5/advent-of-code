import Text.Parsec
import Text.Parsec.String (Parser)

type Entry = (Int, [Int])

-- Generate all possible results by inserting + or * between numbers
possibleResults :: [Int] -> [Int]
possibleResults []     = []
possibleResults [x]    = [x]
possibleResults (x:xs) = go x xs
    where
        go acc [] = [acc]
        go acc (y:ys) = go (acc + y) ys ++
            go (acc * y) ys ++
            go (read $ show acc ++ show y) ys

-- Check if target can be achieved
canReachTarget :: Entry -> Bool
canReachTarget (target, nums) = target `elem` possibleResults nums

int :: Parser Int
int = read <$> many1 digit

space' :: Parser Char
space' = char ' '

entry :: Parser Entry
entry = do
    target <- int
    char ':'
    nums <- many1 (space' >> int)
    newline
    return (target, nums)

list :: Parser [Entry]
list = many1 entry

daySeven :: Int -> FilePath -> IO ()
daySeven n fname = do
    input <- readFile fname
    case parse list "" input of
        Right lst -> print $ sum $ map fst $ filter canReachTarget lst
        Left err -> print err


con :: Int -> Int -> Int
con x y = read $ "x" ++ "y"
