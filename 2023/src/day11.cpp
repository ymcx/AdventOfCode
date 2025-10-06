#include "misc/io.h"
#include "misc/point.h"
#include "misc/vector.h"
#include <numeric>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

pair<unordered_set<Point>, Point> parse_map(string path) {
  vector<string> lines = read_lines(path);
  Point dimensions = Point(lines.size(), lines[0].size());
  unordered_set<Point> map;

  for (auto [y, line] : views::enumerate(lines)) {
    for (auto [x, item] : views::enumerate(line)) {
      if (item == '#') {
        Point point(y, x);
        map.insert(point);
      }
    }
  }

  return {map, dimensions};
}

unordered_set<int> get_free(const unordered_set<Point> &map, Point dimensions,
                            bool rows) {
  unordered_set<int> free;
  int dimension = rows ? dimensions.y : dimensions.x;

  for (int i = 0; i < dimension; ++i) {
    if (none_of(map.begin(), map.end(),
                [&](Point p) { return (rows ? p.y : p.x) == i; })) {
      free.insert(i);
    }
  }

  return free;
}

pair<long, long> calculate_distance(const unordered_set<Point> &map,
                                    const unordered_set<int> &free_rows,
                                    const unordered_set<int> &free_columns) {
  vector<Point> points(map.begin(), map.end());
  long distance = 0, expansion = 0;

  for (int i = 0; i < points.size(); ++i) {
    Point a = points[i];
    for (int j = i + 1; j < points.size(); ++j) {
      Point b = points[j];

      distance += abs(a.x - b.x);
      distance += abs(a.y - b.y);

      for (int y : free_rows) {
        if ((a.y < y && y < b.y) || (b.y < y && y < a.y)) {
          ++expansion;
        }
      }

      for (int x : free_columns) {
        if ((a.x < x && x < b.x) || (b.x < x && x < a.x)) {
          ++expansion;
        }
      }
    }
  }

  return {distance, expansion};
}

int main(int argc, char *argv[]) {
  auto [map, dimensions] = parse_map(argv[1]);
  auto free_rows = get_free(map, dimensions, true);
  auto free_columns = get_free(map, dimensions, false);
  auto [distance, expansion] = calculate_distance(map, free_rows, free_columns);

  long p1 = distance + expansion;
  long p2 = distance + expansion * (1000000 - 1);

  assert_print(p1, p2, 9609130L, 702152204842L);
}
