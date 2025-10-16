#include "misc/io.h"
#include "misc/point.h"
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

tuple<unordered_set<Point>, Point, Point> parse_map(string path) {
  vector<string> lines = read_lines(path);

  unordered_set<Point> rocks;
  Point start;
  Point dimensions = Point(lines.size(), lines[0].size());

  for (int y = 0; y < dimensions.y; ++y) {
    for (int x = 0; x < dimensions.x; ++x) {
      Point point = Point(y, x);

      switch (lines[y][x]) {
      case '#':
        rocks.insert(point);
        break;
      case 'S':
        start = point;
        break;
      }
    }
  }

  return {rocks, start, dimensions};
}

unordered_map<Point, int> get_visited(const unordered_set<Point> &rocks,
                                      Point start, Point dimensions) {
  unordered_map<Point, int> visited;
  queue<pair<Point, int>> queue;
  queue.emplace(start, 0);

  while (!queue.empty()) {
    auto [point, steps] = queue.front();
    queue.pop();

    if (rocks.contains(point) || visited.contains(point) ||
        !point.is_valid(dimensions)) {
      continue;
    }

    visited[point] = steps;

    queue.emplace(Point(point.y - 1, point.x), steps + 1);
    queue.emplace(Point(point.y + 1, point.x), steps + 1);
    queue.emplace(Point(point.y, point.x - 1), steps + 1);
    queue.emplace(Point(point.y, point.x + 1), steps + 1);
  }

  return visited;
}

tuple<long, long, long, long>
get_squares(const unordered_map<Point, int> &visited, Point dimensions) {
  long even_corners = 0;
  long odd_corners = 0;
  long even_full = 0;
  long odd_full = 0;

  for (auto [_, value] : visited) {
    even_corners += value % 2 == 0 && value > dimensions.x / 2;
    odd_corners += value % 2 == 1 && value > dimensions.x / 2;
    even_full += value % 2 == 0;
    odd_full += value % 2 == 1;
  }

  return {even_corners, odd_corners, even_full, odd_full};
}

// https://github.com/villuna/aoc23
long solve(tuple<long, long, long, long> squares, Point dimensions, bool p2) {
  auto [even_corners, odd_corners, even_full, odd_full] = squares;

  long steps = p2 ? 26501365 : 64;
  long n = steps / dimensions.x;

  long a = (n + 1) * (n + 1) * (p2 ? odd_full : even_full);
  long b = n * n * (p2 ? even_full : odd_full);
  long c = (n + 1) * (p2 ? odd_corners : even_corners);
  long d = n * (p2 ? even_corners : odd_corners);

  return a + b - c + d;
}

int main(int argc, char *argv[]) {
  auto [rocks, start, dimensions] = parse_map(argv[1]);
  unordered_map<Point, int> visited = get_visited(rocks, start, dimensions);
  tuple<long, long, long, long> squares = get_squares(visited, dimensions);

  long p1 = solve(squares, dimensions, false);
  long p2 = solve(squares, dimensions, true);

  assert_print(p1, p2, 3858L, 636350496972143L);
}
