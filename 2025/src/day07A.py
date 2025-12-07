import sys

board = [[char for char in line[:-1]] for line in sys.stdin]
splitters = [[item == "^" for item in row] for row in board]
to_visit = [
    (y, x) for y, row in enumerate(board) for x, item in enumerate(row) if item == "S"
]

height = len(board)
visited = set()
result = 0

while len(to_visit) != 0:
    y, x = to_visit.pop()

    while y < height and (y, x) not in visited:
        visited.add((y, x))

        if splitters[y][x]:
            result += 1
            to_visit.append((y, x - 1))
            to_visit.append((y, x + 1))
            break

        y += 1

print(result)
