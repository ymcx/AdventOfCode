import sys


def contains(ranges, item):
    for range in ranges:
        if range[0] <= item <= range[1]:
            return True

    return False


def split_newline(item):
    return item.split("\n")


text = sys.stdin.read()[:-1]
ranges, items = map(split_newline, text.split("\n\n", 1))

ranges = [list(map(int, range.split("-", 1))) for range in ranges]
items = list(map(int, items))

count = sum(contains(ranges, item) for item in items)
print(count)
