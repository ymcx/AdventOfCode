#include "misc/io.h"
#include "misc/point.h"
#include "misc/vector.h"
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

optional<int> extract(string line, Point p, unordered_set<Point> *checked) {
  int start = p.x;
  while (start > 0 && isdigit(line[start - 1])) {
    --start;
  }

  int end = p.x;
  while (end + 1 < line.length() && isdigit(line[end + 1])) {
    ++end;
  }

  Point id(p.y, start);
  if (checked->contains(id)) {
    return nullopt;
  }

  checked->insert(id);

  string number = line.substr(start, end - start + 1);
  return stoi(number);
}

vector<int> closest_numbers(const vector<string> &board,
                            const unordered_set<Point> &points, bool p2) {
  vector<int> results;
  unordered_set<Point> checked;
  int y_max = board.size();
  int x_max = board[0].size();

  for (Point p : points) {
    vector<int> result;
    for (int dy = -1; dy < 2; ++dy) {
      for (int dx = -1; dx < 2; ++dx) {
        int y = p.y + dy;
        int x = p.x + dx;
        if (y < 0 || x < 0 || y >= y_max || x >= x_max) {
          continue;
        }

        char element = board[y][x];
        if (isdigit(element)) {
          string line = board[y];
          Point p(y, x);
          optional<int> number = extract(line, p, &checked);
          if (number.has_value()) {
            result.push_back(number.value());
          }
        }
      }
    }

    if (!p2) {
      results.insert(results.end(), result.begin(), result.end());
    } else if (result.size() == 2) {
      results.push_back(result[0] * result[1]);
    }
  }

  return results;
}

int main(int argc, char *argv[]) {
  vector<string> board = read_lines(argv[1]);

  vector<char> targets_p1 = {'$', '&', '*', '-', '#', '%', '+', '/', '=', '@'};
  unordered_set<Point> coords_p1 = coordinates(board, targets_p1);
  vector<int> numbers_p1 = closest_numbers(board, coords_p1, false);
  int p1 = sum(numbers_p1);

  vector<char> targets_p2 = {'*'};
  unordered_set<Point> coords_p2 = coordinates(board, targets_p2);
  vector<int> numbers_p2 = closest_numbers(board, coords_p2, true);
  int p2 = sum(numbers_p2);

  assert_print(p1, p2, 507214, 72553319);
}
