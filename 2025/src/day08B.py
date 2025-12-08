import sys


def get_euclidean(a, b):
    x1, y1, z1 = a
    x2, y2, z2 = b

    # Taking the square root is unnecessary
    return (x2 - x1) ** 2 + (y2 - y1) ** 2 + (z2 - z1) ** 2


def get_distances():
    distances = {}
    for i in range(len(lines)):
        for j in range(i):
            pair = (i, j)
            distances[pair] = get_euclidean(lines[i], lines[j])

    return sorted(distances.items(), key=lambda x: x[1])


def unite_circuits(pair):
    for i, circuit in enumerate(circuits):
        if pair[0] not in circuit:
            continue

        if pair[1] in circuit:
            return circuits

        break

    for j, circuit in enumerate(circuits):
        if pair[1] not in circuit:
            continue

        circuits[j] += circuits[i]
        del circuits[i]

        break


lines = [list(map(int, line[:-1].split(","))) for line in sys.stdin]
circuits = [[i] for i in range(len(lines))]
distances = get_distances()

i = 0
while 1 < len(circuits):
    pair = distances[i][0]
    unite_circuits(pair)
    i += 1

result = lines[pair[0]][0] * lines[pair[1]][0]

print(result)
