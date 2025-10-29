#include "misc/io.h"
#include "misc/point.h"
#include <ranges>
#include <string>
#include <vector>

using namespace std;

constexpr array<Point, 4> DELTAS = {
    Point(0, 1),
    Point(1, 0),
    Point(0, -1),
    Point(-1, 0),
};

tuple<vector<vector<bool>>, vector<vector<int>>, Point, Point, Point>
parse_board(const string &path) {
  const vector<string> lines = read_lines(path);
  const Point dimensions = Point(lines.size(), lines[0].size());
  vector<vector<bool>> walls(dimensions.y, vector<bool>(dimensions.x, false));
  vector<vector<int>> slopes(dimensions.y, vector<int>(dimensions.x, -1));
  Point start;
  Point end;

  for (const auto &[y, line] : views::enumerate(lines)) {
    for (const auto &[x, item] : views::enumerate(line)) {
      switch (item) {
      case '#':
        walls[y][x] = true;
        break;
      case '.':
        if (y == 0) {
          start = Point(y, x);
        } else if (y == dimensions.y - 1) {
          end = Point(y, x);
        }
        break;
      case '>':
        slopes[y][x] = 0;
        break;
      case 'v':
        slopes[y][x] = 1;
        break;
      case '<':
        slopes[y][x] = 2;
        break;
      case '^':
        slopes[y][x] = 3;
        break;
      }
    }
  }

  return {walls, slopes, start, end, dimensions};
}

int get_longest_path(const vector<vector<bool>> &walls,
                     const vector<vector<int>> &slopes, const Point &start,
                     const Point &end, const Point &dimensions,
                     const int &length, vector<vector<bool>> &visited,
                     const bool &p2) {
  if (start == end) {
    return length;
  }

  visited[start.y][start.x] = true;
  int length_new = 0;

  if (!p2 && slopes[start.y][start.x] != -1) {
    const int slope = slopes[start.y][start.x];
    const Point next = start + DELTAS[slope];

    if (next.is_valid(dimensions) && !walls[next.y][next.x] &&
        !visited[next.y][next.x]) {
      const int length_potential = get_longest_path(
          walls, slopes, next, end, dimensions, length + 1, visited, p2);
      length_new = max(length_new, length_potential);
    }
  } else {
    for (int slope = 0; slope < 4; ++slope) {
      const Point next = start + DELTAS[slope];

      if (next.is_valid(dimensions) && !walls[next.y][next.x] &&
          !visited[next.y][next.x]) {
        const int length_potential = get_longest_path(
            walls, slopes, next, end, dimensions, length + 1, visited, p2);
        length_new = max(length_new, length_potential);
      }
    }
  }

  visited[start.y][start.x] = false;
  return length_new;
}

int main(const int argc, const char *argv[]) {
  const auto [walls, slopes, start, end, dimensions] = parse_board(argv[1]);

  vector<vector<bool>> visited_p1(dimensions.y,
                                  vector<bool>(dimensions.x, false));
  vector<vector<bool>> visited_p2(dimensions.y,
                                  vector<bool>(dimensions.x, false));

  const int p1 = get_longest_path(walls, slopes, start, end, dimensions, 0,
                                  visited_p1, false);
  const int p2 = get_longest_path(walls, slopes, start, end, dimensions, 0,
                                  visited_p2, true);

  assert_print(p1, p2, 2010, 6318);
}
