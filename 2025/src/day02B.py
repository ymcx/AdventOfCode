import sys

line = sys.stdin.readline()
ranges = [tuple(map(int, range.split("-", 1))) for range in line[:-1].split(",")]
values = [str(value) for values in ranges for value in range(values[0], values[1] + 1)]

sum = 0
for value in values:
    length = len(value)

    if any(
        all(value[i : i + j] == value[0:j] for i in range(j, length, j))
        for j in range(1, length // 2 + 1)
    ):
        sum += int(value)

print(sum)
