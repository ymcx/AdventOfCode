#include "misc/io.h"
#include "misc/point.h"
#include <ranges>
#include <string>
#include <unordered_set>

using namespace std;

tuple<unordered_set<Point>, unordered_set<Point>, Point>
parse_rocks(string path) {
  vector<string> lines = read_lines(path);
  unordered_set<Point> rocks;
  unordered_set<Point> rocks_square;
  Point dimensions = Point(lines.size(), lines[0].size());

  for (auto [y, line] : views::enumerate(lines)) {
    for (auto [x, item] : views::enumerate(line)) {
      Point point = Point(y, x);
      switch (item) {
      case ('O'):
        rocks.insert(point);
        break;
      case ('#'):
        rocks_square.insert(point);
        break;
      }
    }
  }

  return {rocks, rocks_square, dimensions};
}

unordered_set<Point> move_p1(const unordered_set<Point> &rocks,
                             const unordered_set<Point> &rocks_square,
                             Point dimensions, Point delta) {
  unordered_set<Point> rocks_new;

  for (Point rock : rocks) {
    while (rocks_new.contains(rock)) {
      rock = rock - delta;
    }

    Point rock_new = rock;
    while (true) {
      rock = rock + delta;

      if (rock.y < 0 || rock.x < 0 || rock.y >= dimensions.y ||
          rock.x >= dimensions.x) {
        break;
      }

      if (rocks_square.contains(rock)) {
        break;
      }

      if (rocks_new.contains(rock)) {
        continue;
      }

      rock_new = rock;
    }

    rocks_new.insert(rock_new);
  }

  return rocks_new;
}

int calculate_score(const unordered_set<Point> &rocks, Point dimensions) {
  int count = 0;

  for (Point rock : rocks) {
    count += dimensions.y - rock.y;
  }

  return count;
}

unordered_set<Point> move_p2(unordered_set<Point> &rocks,
                             const unordered_set<Point> &rocks_square,
                             Point dimensions) {
  vector<unordered_set<Point>> rocks_all;

  for (int i = 0; true; ++i) {
    rocks = move_p1(rocks, rocks_square, dimensions, {-1, 0});
    rocks = move_p1(rocks, rocks_square, dimensions, {0, -1});
    rocks = move_p1(rocks, rocks_square, dimensions, {1, 0});
    rocks = move_p1(rocks, rocks_square, dimensions, {0, 1});

    for (int j = 0; j < rocks_all.size(); ++j) {
      if (rocks_all[j] == rocks && (999999999 - j) % (i - j) == 0) {
        return rocks;
      }
    }

    rocks_all.push_back(rocks);
  }
}

int main(int argc, char *argv[]) {
  auto [rocks, rocks_square, dimensions] = parse_rocks(argv[1]);

  unordered_set<Point> rocks_p1 =
      move_p1(rocks, rocks_square, dimensions, {-1, 0});
  unordered_set<Point> rocks_p2 = move_p2(rocks, rocks_square, dimensions);

  int p1 = calculate_score(rocks_p1, dimensions);
  int p2 = calculate_score(rocks_p2, dimensions);

  assert_print(p1, p2, 108813, 104533);
}
