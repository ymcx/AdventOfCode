#include "misc/io.h"
#include "misc/point.h"
#include "misc/string.h"
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int get_direction(char direction) {
  switch (direction) {
  case 'R':
  case '0':
    return RIGHT;
  case 'D':
  case '1':
    return DOWN;
  case 'L':
  case '2':
    return LEFT;
  case 'U':
  case '3':
    return UP;
  default:
    return -1;
  }
}

vector<DirectionalPoint> get_points(const vector<string> &lines, bool p2) {
  DirectionalPoint point = DirectionalPoint();
  vector<DirectionalPoint> points;

  for (string line : lines) {
    vector<string> parts = split_space(line);

    char direction_char = p2 ? parts[2][7] : parts[0][0];
    int direction = get_direction(direction_char);

    string steps_string = p2 ? parts[2].substr(2, 5) : parts[1];
    int base = p2 ? 16 : 10;
    int steps = stoi(steps_string, 0, base);

    for (int i = 0; i < steps; ++i) {
      point.move_inplace(direction);
      points.push_back(point);
    }
  }

  return points;
}

long calculate_area(const vector<DirectionalPoint> &points) {
  long area = 0;

  for (int i = 1; i < points.size(); ++i) {
    area += points[i - 1].x * points[i].y - points[i].x * points[i - 1].y;
  }

  area += points.back().x * points[0].y - points[0].x * points.back().y;
  area += points.size();
  area /= 2;
  area += 1;

  return area;
}

int main(int argc, char *argv[]) {
  vector<string> lines = read_lines(argv[1]);
  vector<DirectionalPoint> points_p1 = get_points(lines, false);
  vector<DirectionalPoint> points_p2 = get_points(lines, true);

  long p1 = calculate_area(points_p1);
  long p2 = calculate_area(points_p2);

  assert_print(p1, p2, 46359L, 59574883048274L);
}
