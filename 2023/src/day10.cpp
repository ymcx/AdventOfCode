#include "misc/io.h"
#include "misc/point.h"
#include <regex>
#include <string>
#include <vector>

using namespace std;

Point get_next(Point point, Point dp, const vector<vector<char>> &map) {
  int max_y = map.size();
  int max_x = map[0].size();

  char item = map[point.y][point.x];
  char right, left, up, down;
  switch (item) {
  case ('S'):
    if (point.x + 1 < max_x) {
      right = map[point.y][point.x + 1];
      if (right == '-' || right == 'J' || right == '7') {
        return Point(0, 1);
      }
    }
    if (point.x - 1 >= 0) {
      left = map[point.y][point.x - 1];
      if (left == '-' || left == 'F' || left == 'L') {
        return Point(0, -1);
      }
    }
    if (point.y - 1 >= 0) {
      up = map[point.y - 1][point.x];
      if (up == '|' || up == 'F' || up == '7') {
        return Point(-1, 0);
      }
    }
    if (point.y + 1 < max_y) {
      down = map[point.y + 1][point.x];
      if (down == '|' || down == 'J' || down == 'L') {
        return Point(1, 0);
      }
    }
  case ('|'):
    if (dp == Point(-1, 0)) {
      return Point(-1, 0);
    } else {
      return Point(1, 0);
    }
  case ('-'):
    if (dp == Point(0, 1)) {
      return Point(0, 1);
    } else {
      return Point(0, -1);
    }
  case ('F'):
    if (dp == Point(-1, 0)) {
      return Point(0, 1);
    } else {
      return Point(1, 0);
    }
  case ('J'):
    if (dp == Point(0, 1)) {
      return Point(-1, 0);
    } else {
      return Point(0, -1);
    }
  case ('L'):
    if (dp == Point(1, 0)) {
      return Point(0, 1);
    } else {
      return Point(-1, 0);
    }
  case ('7'):
    if (dp == Point(-1, 0)) {
      return Point(0, -1);
    } else {
      return Point(1, 0);
    }
  }
  return Point(0, 0);
}

void check_grid(const vector<vector<char>> &map, Point starting_point) {
  int max_y = map.size();
  int max_x = map[0].size();

  Point cur = starting_point;
  Point lastd = Point(0, 0);

  int i = 0;
  while (true) {
    Point nextd = get_next(cur, lastd, map);
    lastd = nextd;
    cur = cur + nextd;

    if (map[cur.y][cur.x] == 'S') {
      break;
    }
    ++i;
  }
  println((i + 1) / 2);
}

int main(int argc, char *argv[]) {
  vector<string> lines = read_lines(argv[1]);
  Point starting_point;
  vector<vector<char>> map;

  for (auto [y, line] : views::enumerate(lines)) {
    map.emplace_back(line.begin(), line.end());

    for (auto [x, item] : views::enumerate(line)) {
      if (item == 'S') {
        starting_point = Point(y, x);
      }
    }
  }
  check_grid(map, starting_point);
}
