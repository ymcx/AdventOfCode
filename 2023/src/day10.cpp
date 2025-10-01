#include "misc/io.h"
#include "misc/point.h"
#include <cstring>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

Point get_next_delta(const vector<vector<char>> &map, Point current,
                     Point delta) {
  Point up = Point(-1, 0);
  Point down = Point(1, 0);
  Point left = Point(0, -1);
  Point right = Point(0, 1);

  switch (map[current.y][current.x]) {
  case ('|'):
    return (delta == up) ? up : down;
  case ('-'):
    return (delta == right) ? right : left;
  case ('F'):
    return (delta == up) ? right : down;
  case ('L'):
    return (delta == down) ? right : up;
  case ('7'):
    return (delta == right) ? down : left;
  case ('J'):
    return (delta == right) ? up : left;
  }

  return Point(0, 0);
}

unordered_set<Point> get_path(const vector<vector<char>> &map, Point start) {
  unordered_set<Point> path;
  Point current = start;
  Point delta;

  do {
    Point next_delta = get_next_delta(map, current, delta);
    delta = next_delta;
    current = current + next_delta;
    path.insert(current);
  } while (current != start);

  return path;
}

int get_inside_area(const vector<vector<char>> &map,
                    const unordered_set<Point> &path) {
  unordered_set<Point> inside_area;

  for (auto [y, line] : views::enumerate(map)) {
    bool inside = false;
    char last;

    for (auto [x, item] : views::enumerate(line)) {
      Point current = Point(y, x);

      if (path.contains(current)) {
        if ((item == 'J' && last == 'F') || (item == '7' && last == 'L') ||
            (item == '|')) {
          inside ^= true;
        }

        if (strchr("F7JL|", item)) {
          last = item;
        }

        continue;
      }

      if (inside) {
        inside_area.insert(current);
      }
    }
  }

  return inside_area.size();
}

vector<vector<char>> transform_chars(const vector<string> &input) {
  vector<vector<char>> output;
  for (string i : input) {
    output.emplace_back(i.begin(), i.end());
  }

  return output;
}

Point get_start(const vector<vector<char>> &map) {
  for (auto [y, line] : views::enumerate(map)) {
    for (auto [x, item] : views::enumerate(line)) {
      if (item == 'S') {
        return Point(y, x);
      }
    }
  }

  return Point(0, 0);
}

bool connects_left(char c) { return strchr("L-F", c); }

bool connects_right(char c) { return strchr("J-7", c); }

bool connects_up(char c) { return strchr("F|7", c); }

bool connects_down(char c) { return strchr("J|L", c); }

char determine_pipe(const vector<vector<char>> &map, Point point) {
  char left = map[point.y][point.x - 1];
  char right = map[point.y][point.x + 1];
  char up = map[point.y - 1][point.x];
  char down = map[point.y + 1][point.x];

  if (connects_left(left) && connects_right(right))
    return '-';
  if (connects_up(up) && connects_down(down))
    return '|';
  if (connects_left(left) && connects_up(up))
    return 'J';
  if (connects_left(left) && connects_down(down))
    return '7';
  if (connects_right(right) && connects_up(up))
    return 'L';
  if (connects_right(right) && connects_down(down))
    return 'F';

  return ' ';
}

int main(int argc, char *argv[]) {
  vector<string> lines = read_lines(argv[1]);
  vector<vector<char>> map = transform_chars(lines);
  Point start = get_start(map);
  map[start.y][start.x] = determine_pipe(map, start);
  unordered_set<Point> path = get_path(map, start);

  int p1 = path.size() / 2;
  int p2 = get_inside_area(map, path);

  assert_print(p1, p2, 6903, 265);
}
