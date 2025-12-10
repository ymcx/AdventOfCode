import sys
import itertools


def find_amount_presses(presses):
    for permutation in itertools.permutations(buttons, presses):
        counter = diagram

        for i, button in enumerate(permutation, 1):
            counter ^= button

            if counter == 0 and i < presses:
                return i

    return presses


def get_diagram(line):
    diagram = line.split(" ", 1)[0][1:-1]
    diagram = diagram.replace("#", "1").replace(".", "0")
    diagram = diagram[::-1]
    diagram = int(diagram, 2)

    return diagram


def get_buttons(line):
    buttons = line.split(" ")[1:-1]
    buttons = [button[1:-1] for button in buttons]
    buttons = [[int(i) for i in button.split(",")] for button in buttons]
    buttons = [sum(1 << i for i in button) for button in buttons]

    return buttons


result = 0
for line in sys.stdin:
    diagram = get_diagram(line[:-1])
    buttons = get_buttons(line[:-1])

    presses = len(buttons)
    while (p := find_amount_presses(presses)) != presses:
        presses = p

    result += presses

print(result)
