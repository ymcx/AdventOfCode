import sys

sum = 0
for line in sys.stdin:
    numbers = [int(char) for char in line[:-1]]
    first = max(numbers[:-1])
    index = numbers.index(first)
    second = max(numbers[index + 1 :])

    sum += first * 10 + second

print(sum)
