import sys
import pulp
from pulp import LpProblem, LpVariable, PULP_CBC_CMD


def get_joltage(line):
    joltage = line.split(" ")[-1][1:-1]
    joltage = [int(i) for i in joltage.split(",")]

    return joltage


def get_buttons(line, joltage):
    buttons = line.split(" ")[1:-1]
    buttons = [button[1:-1] for button in buttons]
    buttons = [[int(i) for i in button.split(",")] for button in buttons]
    buttons = [[int(i in button) for i in range(len(joltage))] for button in buttons]

    return buttons


def solve(joltage, buttons):
    model = LpProblem()

    x = [LpVariable(f"x{i}", lowBound=0, cat="Integer") for i in range(len(buttons))]
    model += pulp.lpSum(x)

    for i in range(len(joltage)):
        components = [button[i] * x for button, x in zip(buttons, x)]
        model += pulp.lpSum(components) == joltage[i]

    arguments = PULP_CBC_CMD(msg=False)
    model.solve(arguments)
    solution = sum(int(variable.value()) for variable in x)

    return solution


result = 0
for line in sys.stdin:
    joltage = get_joltage(line[:-1])
    buttons = get_buttons(line[:-1], joltage)
    result += solve(joltage, buttons)

print(result)
