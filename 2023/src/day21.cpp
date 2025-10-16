#include "misc/io.h"
#include "misc/point.h"
#include "misc/vector.h"
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

bool is_div(int i) { return i % 2 == 0; }
int main(int argc, char *argv[]) {
  vector<string> lines = read_lines(argv[1]);
  Point dimensions = Point(lines.size(), lines[0].size());
  unordered_set<Point> rocks;
  Point start;

  for (int y = 0; y < dimensions.y; ++y) {
    for (int x = 0; x < dimensions.x; ++x) {
      Point point = Point(y, x);
      if (lines[y][x] == '#') {
        rocks.insert(point);
      }
      if (lines[y][x] == 'S') {
        start = point;
      }
    }
  }

  queue<pair<Point, int>> positions;
  unordered_map<Point, int> already_travelled;
  positions.push({start, stoi(argv[2])});
  while (!positions.empty()) {
    auto [pos, left] = positions.front();
    positions.pop();

    if (left < 0 || rocks.contains(pos) ||
        (already_travelled.contains(pos) && already_travelled[pos] >= left)) {
      continue;
    }

    already_travelled[pos] = left;

    if (left <= 0) {
      continue;
    }

    left -= 1;

    positions.push({pos + Point(1, 0), left});
    positions.push({pos + Point(-1, 0), left});
    positions.push({pos + Point(0, 1), left});
    positions.push({pos + Point(0, -1), left});
  }

  for (int y = 0; y < dimensions.y; ++y) {
    for (int x = 0; x < dimensions.x; ++x) {
      Point point = Point(y, x);
      if (rocks.contains(point)) {
        print("#");
      } else if (already_travelled.contains(point)) {
        if (already_travelled[point] >= 10) {
          print(to_string(already_travelled[point])[0]);

        } else {
          print(already_travelled[point]);
        }
      } else {
        print(" ");
      }
    }
    println();
  }

  vector<int> vals = values(already_travelled);
  vals = filter(vals, is_div);
  int p1 = vals.size();
  println(p1);
}
