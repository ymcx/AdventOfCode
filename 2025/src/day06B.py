import sys
import math

lines = sys.stdin.readlines()
lines = [line[:-1] for line in lines]

height = len(lines)
width = len(lines[0])

result = 0
values = []
multiply = False

for x in range(width):
    if lines[-1][x] != " ":
        result += math.prod(values) if multiply else sum(values)
        values = []
        multiply = lines[-1][x] == "*"

    value = ""
    for y in range(height - 1):
        if lines[y][x] != " ":
            value += lines[y][x]

    if value:
        value = int(value)
        values.append(value)

result += math.prod(values) if multiply else sum(values)

print(result)
