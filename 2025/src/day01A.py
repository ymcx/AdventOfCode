import sys

current = 50
count = 0

for line in sys.stdin:
    dir = line[0]
    amount = int(line[1:-1])
    change = -amount if dir == "L" else amount

    current = (current + change) % 100

    if current == 0:
        count += 1

print(count)
