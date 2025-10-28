#include "misc/io.h"
#include "misc/point.h"
#include <algorithm>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

tuple<unordered_set<Point>, unordered_map<Point, char>, Point, Point, Point>
parse_board(const string &path) {
  vector<string> lines = read_lines(path);
  unordered_set<Point> walls;
  unordered_map<Point, char> slopes;
  Point start;
  Point end;
  Point dimensions = Point(lines.size(), lines[0].size());

  for (auto [y, line] : views::enumerate(lines)) {
    for (auto [x, item] : views::enumerate(line)) {
      Point point = Point(y, x);
      switch (item) {
      case '#':
        walls.insert(point);
        break;
      case '.':
        if (y == 0) {
          start = point;
        }
        if (y == lines.size() - 1) {
          end = point;
        }
        break;
      case '>':
      case '<':
      case '^':
      case 'v':
        slopes[point] = item;
        break;
      }
    }
  }
  return {walls, slopes, start, end, dimensions};
}

int get_longest_path(unordered_set<Point> &walls,
                     unordered_map<Point, char> &slopes, Point start, Point end,
                     Point dimensions, int startint,

                     unordered_map<Point, int> visited

) {
  queue<pair<Point, int>> to_travel({{start, startint}});

  int maxx = 0;
  while (!to_travel.empty()) {

    auto [point, cur] = to_travel.front();
    to_travel.pop();

    print(point);
    print(" ");
    print(cur);
    println();

    if (!point.is_valid(dimensions) || walls.contains(point) ||
        (visited.contains(point))) {
      continue;
    }

    if (point == end) {

      maxx = std::max(maxx, cur);
      continue;
    }

    visited.emplace(point, cur);
    cur += 1;

    if (slopes.contains(point)) {
      int result2;
      Point p;
      switch (slopes[point]) {
      case '>':

        p = point + Point(0, 1);
        result2 =
            get_longest_path(walls, slopes, p, end, dimensions, cur, visited);
        maxx = std::max(maxx, result2);
        break;
      case '<':

        p = point + Point(0, -1);
        result2 =
            get_longest_path(walls, slopes, p, end, dimensions, cur, visited);
        maxx = std::max(maxx, result2);
        break;
      case 'v':

        p = point + Point(1, 0);
        result2 =
            get_longest_path(walls, slopes, p, end, dimensions, cur, visited);
        maxx = std::max(maxx, result2);
        break;
      case '^':
        p = point + Point(-1, 0);

        result2 =
            get_longest_path(walls, slopes, p, end, dimensions, cur, visited);
        maxx = std::max(maxx, result2);
        break;
      }
      continue;
    }

    int a = 0;
    auto list = {
        Point(1, 0),
        Point(-1, 0),
        Point(0, 1),
        Point(0, -1),
    };
    for (auto d : list) {
      auto p = point + d;
      if (!p.is_valid(dimensions) || walls.contains(p) ||
          (visited.contains(p))) {
        continue;
      }

      if (a > 1) {
        int result =
            get_longest_path(walls, slopes, p, end, dimensions, cur, visited);
        maxx = std::max(maxx, result);

      } else {
        to_travel.emplace(point + d, cur);
      }

      ++a;
    }
  }
  return maxx;
}

int main(const int argc, const char *argv[]) {
  auto [walls, slopes, start, end, dimensions] = parse_board(argv[1]);
  int result = get_longest_path(walls, slopes, start, end, dimensions, 0, {});
  println(result);
}
