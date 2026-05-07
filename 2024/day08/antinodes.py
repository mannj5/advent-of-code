from itertools import combinations

# used haskell to get the unique antenna identifiers
# getUnique s = (filter (`notElem` ".\n")) . nub <$> readFile s
# ^^ that's the function I used

testants = "0A"
inputants = "bAs3VPIvzpuSl8BiEmJcLkaO1Def650TWXGjthCK2gMUqYQnwHo7NFdyZ94x"

def main(filePath: str, ants: str, dim: (int, int)):

    def getPairs(x: char) -> [(int, int)]:
        return [(r, c)
            for r, row in enumerate(grid)
            for c, v in enumerate(row)
            if v == x]

    rows, cols = dim
    grid = [line.strip() for line in open(filePath)]
    positions = [getPairs(x) for x in ants]

    # --------
    # PART 1
    # --------
    s1 = set()
    for nodes in positions:
        # combinations automatically orders the tuples so that they
        # are in ascending order as well, which allows for much less logic
        for (x1, y1), (x2, y2) in combinations(nodes, 2):
            # calculate difference in x and y
            dx = x2 - x1
            dy = y2 - y1

            # calculate step below the 'smaller' node
            ax = x1 - dx
            ay = y1 - dy

            # calculate step above the 'larger' node
            bx = x2 + dx
            by = y2 + dy

            # check if they are in bounds and add to set if yes
            if 0 <= ax < cols and 0 <= ay < rows:
                s1.add((ax, ay))
            if 0 <= bx < cols and 0 <= by < rows:
                s1.add((bx, by))
        
    print(len(s1))

    # --------
    # PART 2
    # --------
    s2 = set()
    for nodes in positions:
        # combinations automatically orders the tuples so that they
        # are in ascending order as well, which allows for much less logic
        for (x1, y1), (x2, y2) in combinations(nodes, 2):
            # calculate difference in x and y
            dx = x2 - x1
            dy = y2 - y1

            # calculate step below the 'smaller' node
            ax = x1 
            ay = y1
            while 0 <= ax < cols and 0 <= ay < rows:
                s2.add((ax, ay))
                ax -= dx
                ay -= dy

            # calculate step above the 'larger' node
            bx = x2
            by = y2
            while 0 <= bx < cols and 0 <= by < rows:
                s2.add((bx, by))
                bx += dx
                by += dy
        
    print(len(s2)) 

main("d8input", inputants, (50, 50))