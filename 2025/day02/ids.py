import sys
import re

def map_ranges(filename: str) -> [(int, int)]:

    ranges = []
    with open(filename) as f:
        lines = f.read().split(',')
        for line in lines:
            start, end = [int(p) for p in line.split('-')]
            ranges.append((start, end))

    return ranges

def search_match(ranges, pattern):
    total = 0
    for start, end in ranges:
        for i in range(start, end+1):
            if re.match(pattern, str(i)):
                total += i

    return total


def part_one(ranges: [(int, int)]) -> None:
    
    INVALID_ID_PATTERN = r'^(\d+)\1$'
    total = search_match(ranges, INVALID_ID_PATTERN)
    
    print("part 1: " + str(total))

def part_two(ranges: [(int, int)]) -> None:

    INVALID_ID_PATTERN = r'^(\d+)(\1)+$'
    total = search_match(ranges, INVALID_ID_PATTERN)
    
    print("part 2: " + str(total))

ranges = map_ranges(sys.argv[1])
part_one(ranges)
part_two(ranges)