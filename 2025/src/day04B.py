import sys


def is_paper(board, y, x):
    return 0 <= y < len(board) and 0 <= x < len(board[0]) and board[y][x]


def can_access(board, y, x):
    paper = 0
    for i in range(0, 9):
        dy = i // 3 - 1
        dx = i % 3 - 1
        paper += is_paper(board, y + dy, x + dx)

    if paper <= 4:
        board[y][x] = False
        return True

    return False


board = []
for line in sys.stdin:
    row = [char == "@" for char in line[:-1]]
    board.append(row)

count = 0
last = -1
while count != last:
    last = count
    for y, row in enumerate(board):
        for x, paper in enumerate(row):
            count += paper and can_access(board, y, x)

print(count)
