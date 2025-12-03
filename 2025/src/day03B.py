import sys

sum = 0
for line in sys.stdin:
    numbers = [int(char) for char in line[:-1]]

    start = 0
    for i in range(0, 12):
        end = len(numbers) - 11 + i
        number = max(numbers[start:end])
        start += numbers[start:end].index(number) + 1

        sum += number * 10 ** (11 - i)

print(sum)
