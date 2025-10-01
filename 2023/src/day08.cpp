#include "misc/io.h"
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

tuple<vector<bool>, unordered_map<string, pair<string, string>>, vector<string>>
parse_instructions(string path) {
  vector<string> lines = read_lines(path);
  vector<bool> movements;
  unordered_map<string, pair<string, string>> paths;
  vector<string> starting_points;

  for (char c : lines[0]) {
    movements.push_back(c == 'R');
  }

  for (int i = 2; i < lines.size(); ++i) {
    string line = lines[i];
    string key = line.substr(0, 3);
    string lvalue = line.substr(7, 3);
    string rvalue = line.substr(12, 3);

    if (key.ends_with('A')) {
      starting_points.push_back(key);
    }

    paths[key] = {lvalue, rvalue};
  }

  return {movements, paths, starting_points};
}

int solve(const vector<bool> &movements,
          const unordered_map<string, pair<string, string>> &paths,
          string current, bool p2) {
  int i = 0;

  while (true) {
    for (bool movement : movements) {
      if ((p2 && current.ends_with('Z')) || (!p2 && current == "ZZZ")) {
        return i;
      }

      pair<string, string> next = paths.at(current);
      current = movement ? next.second : next.first;

      ++i;
    }
  }
}

long solve_p2(const vector<bool> &movements,
              const unordered_map<string, pair<string, string>> &paths,
              const vector<string> &starting_points) {
  vector<int> results;

  for (string current : starting_points) {
    int result = solve(movements, paths, current, true);
    results.push_back(result);
  }

  return accumulate(results.begin(), results.end(), 1L, lcm<long, long>);
}

int main(int argc, char *argv[]) {
  auto [movements, paths, starting_points] = parse_instructions(argv[1]);

  long p1 = solve(movements, paths, "AAA", false);
  long p2 = solve_p2(movements, paths, starting_points);

  assert_print(p1, p2, 22199L, 13334102464297L);
}
