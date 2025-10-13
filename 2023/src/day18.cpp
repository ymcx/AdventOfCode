#include "misc/io.h"
#include "misc/point.h"
#include "misc/string.h"
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  vector<string> lines = read_lines(argv[1]);

  DirectionalPoint point = DirectionalPoint();
  int direction = 0;
  int total = 0;

  vector<DirectionalPoint> points;
  points.push_back(point);

  for (const string &line : lines) {
    switch (line[0]) {
    case 'L':
      direction = LEFT;
      break;
    case 'R':
      direction = RIGHT;
      break;
    case 'U':
      direction = UP;
      break;
    case 'D':
      direction = DOWN;
      break;
    }

    int steps = stoi(split_space(line)[1]);

    for (int d = 0; d < steps; ++d) {
      point.move_inplace(direction);
      points.push_back(point);
    }
  }
  for (int i = 1; i < points.size(); ++i) {
    total += points[i - 1].x * points[i].y - points[i].x * points[i - 1].y;
  }
  total +=
      points.back().x * points.front().y - points.front().x * points.back().y;

  println(total / 2 + points.size() / 2 + 1);
}
