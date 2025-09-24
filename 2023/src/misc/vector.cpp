#include "vector.h"
#include "point.h"
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

unordered_set<Point> coordinates(const vector<string> &board,
                                 const vector<char> &targets) {
  unordered_set<Point> coordinates;

  for (auto [y, line] : views::enumerate(board)) {
    for (auto [x, element] : views::enumerate(line)) {
      if (find(targets.begin(), targets.end(), element) != targets.end()) {
        Point p(y, x);
        coordinates.insert(p);
      }
    }
  }

  return coordinates;
}
