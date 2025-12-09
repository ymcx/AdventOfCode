import sys
from shapely.geometry import Polygon


def get_distance(a, b):
    x1, y1 = a
    x2, y2 = b

    dx = abs(x2 - x1)
    dy = abs(y2 - y1)

    return (dx + 1) * (dy + 1)


def get_rectangle(a, b):
    rectangle = [
        (a[0], a[1]),
        (a[0], b[1]),
        (b[0], b[1]),
        (b[0], a[1]),
    ]

    return Polygon(rectangle)


result = 0
coordinates = [
    tuple(map(int, line[:-1].split(",", 1))) for line in sys.stdin.readlines()
]
polygon = Polygon(coordinates)

for i in range(len(coordinates)):
    for j in range(i):
        distance = get_distance(coordinates[i], coordinates[j])
        if distance <= result:
            continue

        rectangle = get_rectangle(coordinates[i], coordinates[j])
        if not polygon.contains(rectangle):
            continue

        result = distance

print(result)
