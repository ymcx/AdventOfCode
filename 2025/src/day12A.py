import sys
import copy

lines = sys.stdin.read()[:-1].split("\n\n")

shapes = [line.split()[1:] for line in lines[:-1]]
shapes = [[[item == "#" for item in row] for row in shape] for shape in shapes]

regions = [line.split(": ", 1) for line in lines[-1].split("\n")]
regions = [
    [list(map(int, ll.split("x", 1))), list(map(int, region.split()))]
    for ll, region in regions
]

def do_they_fit(width,length,region33,region43,region44,region53):
    region33 = copy.deepcopy(region33)
    region43 = copy.deepcopy(region43)
    region44 = copy.deepcopy(region44)
    region53 = copy.deepcopy(region53)

    ww,ll=0,3

    while region33:
        for _ in range(region33.pop()):
            ww += 3
            if ww>width:
                ww = 3
                ll += 3

    while region43:
        for _ in range(region43.pop()):
            ww += 4
            if ww>width:
                ww = 4
                ll += 3

    while region53:
        for _ in range(region53.pop()):
            ww += 5
            if ww>width:
                ww = 5
                ll += 3


    while region44:
        ll+=1
        for _ in range(region44.pop()):
            ww += 4
            if ww>width:
                ww = 4
                ll += 4


        
    return ww <= width and ll <= length

count = 0
for [width, length], region33 in regions:
    region33 = [i for i in region33]
    region43 = []
    region44 = []
    region53 = []

    r = min(region33[4], region33[5])
    region33[4] -= r
    region33[5] -= r
    region43.append(r)

    r = min(region33[0], region33[1])
    region33[0] -= r
    region33[1] -= r
    region53.append(r)

    region44.append(region33[3] // 2)
    region33[3] = region33[3] % 2

    fit = do_they_fit(width,length,region33,region43,region44,region53)

    print(f"{region33}\t{region43}\t{region44}\t{region53}\t{width}\t{length}\t{fit}")

    if fit:
        count += 1

print("33\t\t\t43\t44\t53\twidth\theight\tfit")
print()
print(count)
