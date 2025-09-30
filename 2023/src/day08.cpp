#include "misc/io.h"
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

pair<vector<bool>, unordered_map<string, pair<string, string>>>
parse_instructions(string path) {
  vector<string> lines = read_lines(path);

  vector<bool> movements;
  for (char c : lines[0]) {
    movements.push_back(c == 'R');
  }

  unordered_map<string, pair<string, string>> map;
  for (int i = 2; i < lines.size(); ++i) {
    string line = lines[i];
    string key = line.substr(0, 3);
    string l_value = line.substr(7, 3);
    string r_value = line.substr(12, 3);
    map[key] = {l_value, r_value};
  }


  return {movements, map};
}

int solve(const vector<bool> &movements,
          const unordered_map<string, pair<string, string>> &map) {
  string current = "AAA";
  int count = 0;
  while (true) {
    for (bool movement : movements) {
      if (current == "ZZZ") {
        return count;
      }
      ++count;
      if (movement) {
        current = map.at(current).second;
      } else {
        current = map.at(current).first;
      }
    }
  }
  return count;
}
int main(int argc, char *argv[]) {
  auto [movements, map] = parse_instructions(argv[1]);
  int val = solve(movements, map);
  println(val);
}
