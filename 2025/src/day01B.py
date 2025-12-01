import sys

current = 50
count = 0

for line in sys.stdin:
    dir = line[0]
    amount = int(line[1:-1])
    change = -amount if dir == "L" else amount

    if current + change == 0:
        count += 1
    elif current + change < 0 and current != 0:
        count += abs(current + change) // 100 + 1
    else:
        count += abs(current + change) // 100

    current = (current + change) % 100

print(count)
