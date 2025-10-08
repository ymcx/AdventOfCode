#include "misc/io.h"
#include "misc/point.h"
#include "misc/vector.h"
#include <ranges>
#include <string>
#include <vector>

using namespace std;
using Rocks = vector<Point>;

enum Direction { North, South, West, East };

tuple<Rocks, Rocks, Point> parse_rocks(string path) {
  vector<string> lines = read_lines(path);
  vector<Point> rocks_circle;
  vector<Point> rocks_square;
  int max_y = lines.size();
  int max_x = lines[0].length();
  Point dimensions = Point(max_y, max_x);

  for (auto [y, line] : views::enumerate(lines)) {
    for (auto [x, item] : views::enumerate(line)) {
      if (item == '#') {
        rocks_square.push_back(Point(y, x));
      } else if (item == 'O') {
        rocks_circle.push_back(Point(y, x));
      }
    }
  }

  return {rocks_circle, rocks_square, dimensions};
}

void move_all(Rocks &rocks_circle, Rocks &rocks_square, Direction direction,
              Point dimensions) {

  for (Point &rock : rocks_circle) {
    int y = rock.y;
    int x = rock.x;
    int y_new = y;
    int x_new = x;

    while (true) {
      y = y - 1;
      x = x;

      if (y < 0 || x < 0 || y >= dimensions.y || x >= dimensions.x) {
        break;
      }

      if (contains(rocks_circle, {y, x})) {
        continue;
      }
      if (contains(rocks_square, {y, x})) {
        break;
      }

      y_new = y;
      x_new = x;
    }

    rock = {y_new, x_new};
  }
}

int calculate_score(const Rocks &rocks, Point dimensions) {
  int count = 0;

  for (Point rock : rocks) {
    count += dimensions.y - rock.y;
  }

  return count;
}

void print_rocks(Rocks &rocks_circle, Rocks &rocks_square, Point dimensions) {
  for (int y = 0; y < dimensions.y; ++y) {

    for (int x = 0; x < dimensions.x; ++x) {
      if (contains(rocks_circle, {y, x})) {
        print('O');
      } else if (contains(rocks_square, {y, x})) {
        print('#');
      } else {
        print(".");
      }
    }
    println();
  }
  println();
}

int main(int argc, char *argv[]) {
  auto [rocks_circle, rocks_square, dimensions] = parse_rocks(argv[1]);

  print_rocks(rocks_circle, rocks_square, dimensions);

  move_all(rocks_circle, rocks_square, Direction::North, dimensions);

  print_rocks(rocks_circle, rocks_square, dimensions);

  int p1 = calculate_score(rocks_circle, dimensions);
  println(p1);
}
