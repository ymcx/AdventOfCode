#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <regex>
#include <string>
#include <vector>

using namespace std;

vector<pair<int, int>> parse_races(string path) {
  vector<string> lines = read_lines(path);
  lines[0] = regex_replace(lines[0], regex(".+: "), "");
  lines[1] = regex_replace(lines[1], regex(".+: "), "");

  vector<string> parts0l = split(lines[0], " ");
  vector<string> parts1l = split(lines[1], " ");

  vector<int> parts0 = filter_map(parts0l, parse_int);
  vector<int> parts1 = filter_map(parts1l, parse_int);

  vector<pair<int, int>> races;
  races.reserve(parts0.size());

  for (int i = 0; i < parts0.size(); ++i) {
    races.emplace_back(parts0[i], parts1[i]);
  }
  return races;
}

int main(int argc, char *argv[]) {
  vector<pair<int, int>> races = parse_races(argv[1]);

  int total = 1;
  for (auto [time, best] : races) {
    int count = 0;

    for (int hold = 1; hold < time; ++hold) {
      if ((time - hold) * hold > best) {
        ++count;
      }
    }
    total *= count;
  }
  println(total);
}
