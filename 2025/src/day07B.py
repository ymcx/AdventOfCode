import sys

board = [[char for char in line[:-1]] for line in sys.stdin]
splitters = [[item == "^" for item in row] for row in board]
to_visit = [
    (y, x) for y, row in enumerate(board) for x, item in enumerate(row) if item == "S"
]

height = len(board)
cache = {}
y, x = to_visit.pop()


def traverse(y, x):
    if (y, x) in cache:
        return cache[(y, x)]

    if height <= y:
        return 1

    cache[(y, x)] = (
        traverse(y, x + 1) + traverse(y, x - 1)
        if splitters[y][x]
        else traverse(y + 1, x)
    )

    return cache[(y, x)]


result = traverse(y, x)
print(result)
