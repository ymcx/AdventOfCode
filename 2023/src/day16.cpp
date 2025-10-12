#include "misc/io.h"
#include "misc/point.h"
#include <algorithm>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

pair<unordered_map<Point, char>, Point> parse_board(string path) {
  vector<string> lines = read_lines(path);
  unordered_map<Point, char> board;
  Point dimensions = Point(lines.size(), lines[0].size());
  string symbols = "|-/\\";

  for (auto [y, line] : views::enumerate(lines)) {
    for (auto [x, item] : views::enumerate(line)) {
      if (symbols.contains(item)) {
        Point point = Point(y, x);
        board[point] = item;
      }
    }
  }

  return {board, dimensions};
}

unordered_set<DirectionalPoint> get_starts(Point dimensions) {
  unordered_set<DirectionalPoint> starts;

  for (int y = 0; y < dimensions.y; ++y) {
    starts.insert({y, 0, RIGHT});
    starts.insert({y, dimensions.x - 1, LEFT});
  }
  for (int x = 0; x < dimensions.x; ++x) {
    starts.insert({0, x, DOWN});
    starts.insert({dimensions.y - 1, x, UP});
  }

  return starts;
}

int next_directions_slash(int direction) {
  switch (direction) {
  case LEFT:
    return DOWN;
  case RIGHT:
    return UP;
  case UP:
    return RIGHT;
  case DOWN:
    return LEFT;
  default:
    return -1;
  }
}

array<int, 2> next_directions_forwardslash(int direction) {
  direction = next_directions_slash(direction);
  return {direction, -1};
}

array<int, 2> next_directions_backslash(int direction) {
  direction = (next_directions_slash(direction) + 2) % 4;
  return {direction, -1};
}

array<int, 2> next_directions_beam(int direction) {
  switch (direction) {
  case LEFT:
  case RIGHT:
    return {UP, DOWN};
  default:
    return {direction, -1};
  }
}

array<int, 2> next_directions_hyphen(int direction) {
  switch (direction) {
  case UP:
  case DOWN:
    return {LEFT, RIGHT};
  default:
    return {direction, -1};
  }
}

array<int, 2> next_directions(char item, int direction) {
  switch (item) {
  case '|':
    return next_directions_beam(direction);
  case '-':
    return next_directions_hyphen(direction);
  case '/':
    return next_directions_forwardslash(direction);
  case '\\':
    return next_directions_backslash(direction);
  default:
    return {direction, -1};
  }
}

int solve(DirectionalPoint start, Point dimensions,
          const unordered_map<Point, char> &board) {
  unordered_set<Point> positions;
  unordered_set<DirectionalPoint> states;
  queue<DirectionalPoint> unvisited({start});

  while (!unvisited.empty()) {
    DirectionalPoint state = unvisited.front();
    unvisited.pop();

    if (!state.is_valid(dimensions) || states.contains(state)) {
      continue;
    }

    Point position = state.get_position();
    states.insert(state);
    positions.insert(position);

    char item = board.contains(position) ? board.at(position) : '.';
    array<int, 2> directions = next_directions(item, state.direction);
    for (int direction : directions) {
      DirectionalPoint state_next = state.move(direction);
      unvisited.push(state_next);
    }
  }

  return positions.size();
}

int solve(const unordered_set<DirectionalPoint> &starts, Point dimensions,
          const unordered_map<Point, char> &board) {
  int result = 0;

  for (DirectionalPoint start : starts) {
    int potential = solve(start, dimensions, board);
    result = max(result, potential);
  }

  return result;
}

int main(int argc, char *argv[]) {
  auto [board, dimensions] = parse_board(argv[1]);
  DirectionalPoint start = DirectionalPoint(0, 0, RIGHT);
  unordered_set<DirectionalPoint> starts = get_starts(dimensions);

  int p1 = solve(start, dimensions, board);
  int p2 = solve(starts, dimensions, board);

  assert_print(p1, p2, 7798, 8026);
}
