#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

int label(string cards) {
  unordered_map<char, int> map;
  for (char c : cards) {
    ++map[c];
  }

  if (map.size() == 1) {
    return 1;
  }
  if (map.size() == 4) {
    return 6;
  }
  if (map.size() == 5) {
    return 7;
  }

  if (map.size() == 2) {
    vector<int> vals = values(map);
    if (vals[0] == 4 || vals[0] == 1) {
      return 2;
    } else {
      return 3;
    }
  }

  int two = 2;
  if (contains(values(map), two)) {
    return 5;
  }
  return 4;
}

unordered_map<char, int> char_priority = {
    {'A', 0}, {'K', 1}, {'Q', 2}, {'J', 3},  {'T', 4},  {'9', 5}, {'8', 6},
    {'7', 7}, {'6', 8}, {'5', 9}, {'4', 10}, {'3', 11}, {'2', 12}};

bool custom_compare(const std::string &a, const std::string &b) {
  for (int i = 0; i < 5; ++i) {
    int pa = char_priority[a[i]];
    int pb = char_priority[b[i]];
    if (pa != pb)
      return pa > pb;
  }
  return true;
}

int main(int argc, char *argv[]) {
  vector<string> lines = read_lines(argv[1]);
  unordered_map<int, vector<string>> map;

  for (string line : lines) {
    string str = split(line, " ")[0];
    int l = label(str) - 1;
    map[l].push_back(line);
  }

  int results = 0;

  int x = 1;
  for (int i = 6; i >= 0; --i) {
    vector<string> vals = map[i];
    std::sort(vals.begin(), vals.end(), custom_compare);
    for (string v : vals) {
      results += stoi(split(v, " ")[1]) * x;
      ++x;
    }
  }

  println(results);
}
