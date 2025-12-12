import sys


def parse(values):
    return [int(value) for value in values]


def get_regions(text):
    regions = text.split("\n\n")[-1]
    regions = [region.split(": ", 1) for region in regions.split("\n")]
    regions = [
        (parse(dimensions.split("x", 1)), parse(quantities.split(" ")))
        for dimensions, quantities in regions
    ]

    return regions


def add(amount, width, height, x, y, max_x):
    y = max(y, height)

    for _ in range(amount):
        x += width

        if x <= max_x:
            continue

        x = width
        y += height

    return x, y


def fit(max_x, max_y, regions_3x3, regions_4x3, regions_5x3, regions_4x4):
    x, y = 0, 0
    x, y = add(regions_3x3, 3, 3, x, y, max_x)
    x, y = add(regions_4x3, 4, 3, x, y, max_x)
    x, y = add(regions_5x3, 5, 3, x, y, max_x)
    x, y = add(regions_4x4, 4, 4, x, y, max_x)

    return x <= max_x and y <= max_y


text = sys.stdin.read()[:-1]
regions = get_regions(text)

result = 0
for [max_x, max_y], regions_3x3 in regions:
    i = min(regions_3x3[4], regions_3x3[5])
    regions_3x3[4] -= i
    regions_3x3[5] -= i
    regions_4x3 = i

    i = min(regions_3x3[0], regions_3x3[1])
    regions_3x3[0] -= i
    regions_3x3[1] -= i
    regions_5x3 = i

    i = regions_3x3[3] // 2
    regions_3x3[3] -= i * 2
    regions_4x4 = i

    regions_3x3 = sum(regions_3x3)

    result += fit(max_x, max_y, regions_3x3, regions_4x3, regions_5x3, regions_4x4)

print(result)
