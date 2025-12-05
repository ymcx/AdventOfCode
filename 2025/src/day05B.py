import sys


def merge_ranges(ranges):
    ranges = sorted(ranges)

    for i in range(len(ranges) - 1):
        current = ranges[i]
        next = ranges[i + 1]

        if current[1] < next[0] - 1:
            continue

        if current[1] < next[1]:
            ranges[i + 1] = [current[0], next[1]]
        else:
            ranges[i + 1] = current

        ranges[i] = None

    return [range for range in ranges if range]


text = sys.stdin.read()[:-1]
ranges = text.split("\n\n", 1)[0].split("\n")

ranges = [list(map(int, range.split("-", 1))) for range in ranges]
ranges = merge_ranges(ranges)

count = sum(range[1] - range[0] + 1 for range in ranges)
print(count)
