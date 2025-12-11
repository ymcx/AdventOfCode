import sys


def get_connections():
    connections = {}

    for line in sys.stdin:
        input, outputs = line[:-1].split(":", 1)
        connections[input] = outputs.split()

    return connections


def travel(start, end):
    key = (start, end)
    if key in cache:
        return cache[key]

    if start == end:
        return 1

    if start == "out":
        return 0

    cache[key] = sum(travel(start, end) for start in connections[start])
    return cache[key]


cache = {}
connections = get_connections()

result = 1
result *= travel("svr", "fft")
result *= travel("fft", "dac")
result *= travel("dac", "out")

print(result)
