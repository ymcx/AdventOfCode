import sys


def get_distance(a, b):
    x1, y1 = a
    x2, y2 = b

    dx = abs(x2 - x1)
    dy = abs(y2 - y1)

    return (dx + 1) * (dy + 1)


result = 0
coordinates = [
    tuple(map(int, line[:-1].split(",", 1))) for line in sys.stdin.readlines()
]

for i in range(len(coordinates)):
    for j in range(i):
        distance = get_distance(coordinates[i], coordinates[j])
        result = max(result, distance)

print(result)
