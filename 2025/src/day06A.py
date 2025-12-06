import sys
import math

lines = sys.stdin.readlines()
lines = [line.split() for line in lines]

height = len(lines)
width = len(lines[0])

result = 0
for x in range(width):
    values = [int(lines[y][x]) for y in range(height - 1)]
    multiply = lines[-1][x] == "*"
    result += math.prod(values) if multiply else sum(values)

print(result)
