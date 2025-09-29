#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

unordered_map<char, int> p1_priority = {
    {'A', 12}, {'K', 11}, {'Q', 10}, {'J', 9}, {'T', 8}, {'9', 7}, {'8', 6},
    {'7', 5},  {'6', 4},  {'5', 3},  {'4', 2}, {'3', 1}, {'2', 0},
};

unordered_map<char, int> p2_priority = {
    {'A', 12}, {'K', 11}, {'Q', 10}, {'J', -1}, {'T', 8}, {'9', 7}, {'8', 6},
    {'7', 5},  {'6', 4},  {'5', 3},  {'4', 2},  {'3', 1}, {'2', 0},
};

char max_key(const unordered_map<char, int> &map) {
  char max_key = ' ';
  int max_value = 0;
  for (auto [key, value] : map) {
    if (max_value < value) {
      max_value = value;
      max_key = key;
    }
  }

  return max_key;
}

unordered_map<char, int> char_freq(string input, bool p2) {
  unordered_map<char, int> freq;
  for (char c : input) {
    ++freq[c];
  }

  if (p2) {
    freq.erase('J');
    char max = max_key(freq);
    freq[max] += ranges::count(input, 'J');
  }

  return freq;
}

int get_group(string cards, bool p2) {
  unordered_map<char, int> freq = char_freq(cards, p2);
  switch (freq.size()) {
  case (5):
    return 6;
  case (4):
    return 5;
  case (3):
    return (contains(values(freq), 3)) ? 3 : 4;
  case (2):
    return (contains(values(freq), 1)) ? 1 : 2;
  }

  return 0;
}

bool compare(string a, string b, const unordered_map<char, int> &priority) {
  for (int i = 0; i < 5; ++i) {
    int pa = priority.at(a[i]);
    int pb = priority.at(b[i]);
    if (pa != pb) {
      return pa < pb;
    }
  }

  return true;
}

int solve(const vector<string> &lines, bool p2) {
  unordered_map<int, vector<string>> groups;
  unordered_map<string, int> weights;

  for (string line : lines) {
    vector<string> parts = split(line, " ");

    string cards = parts[0];
    weights[cards] = stoi(parts[1]);

    int group = get_group(cards, p2);
    groups[group].push_back(cards);
  }

  unordered_map<char, int> priority = p2 ? p2_priority : p1_priority;
  int result = 0;
  int rank = 0;

  for (int i = 6; i >= 0; --i) {
    vector<string> group = groups[i];
    sort(group.begin(), group.end(),
         [&](string a, string b) { return compare(a, b, priority); });

    for (string cards : group) {
      result += weights[cards] * ++rank;
    }
  }

  return result;
}

int main(int argc, char *argv[]) {
  vector<string> lines = read_lines(argv[1]);
  int p1 = solve(lines, false);
  int p2 = solve(lines, true);
  assert_print(p1, p2, 246424613, 248256639);
}
