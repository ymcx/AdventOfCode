import sys

line = sys.stdin.readline()
ranges = [tuple(map(int, range.split("-", 1))) for range in line[:-1].split(",")]
values = [str(value) for values in ranges for value in range(values[0], values[1] + 1)]

sum = sum(
    int(value)
    for value in values
    if (length := len(value) // 2) and value[:length] == value[length:]
)

print(sum)
