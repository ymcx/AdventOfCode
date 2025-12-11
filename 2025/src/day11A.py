import sys


def get_connections():
    connections = {}

    for line in sys.stdin:
        input, outputs = line[:-1].split(":", 1)
        connections[input] = outputs.split()

    return connections


def get_paths_count(connections):
    to_visit = ["you"]
    paths = 0

    while to_visit:
        key = to_visit.pop()
        if key == "out":
            paths += 1
        else:
            to_visit += connections[key]

    return paths


connections = get_connections()
result = get_paths_count(connections)
print(result)
