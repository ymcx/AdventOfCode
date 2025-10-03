#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// https://github.com/AlexOxorn/AdventOfCode
struct state {
  state *dot;
  state *hash;
};

vector<state> get_states(const vector<int> &amounts) {
  int length = sum(amounts) + amounts.size();
  vector<state> states = vector<state>(length);
  int i = 0;

  states[i].dot = &states[i];
  states[i].hash = &states[i + 1];
  ++i;

  for (int amount : amounts) {
    for (int j = 0; j < amount - 1; ++j) {
      states[i].hash = &states[i + 1];
      ++i;
    }

    if (i > states.size() - 3) {
      break;
    }

    states[i].dot = &states[i + 1];
    ++i;

    states[i].dot = &states[i];
    states[i].hash = &states[i + 1];
    ++i;
  }

  states[i].dot = &states[i];
  return states;
}

long count(const vector<int> amounts, string sequence) {
  vector<state> states = get_states(amounts);
  unordered_map<state *, long> current;
  current[&states[0]] = 1;

  for (char c : sequence) {
    unordered_map<state *, long> next;

    for (auto [key, value] : current) {
      if ((c == '.' || c == '?') && key->dot) {
        next[key->dot] += value;
      }

      if ((c == '#' || c == '?') && key->hash) {
        next[key->hash] += value;
      }
    }

    current = next;
  }

  return current[&states.back()];
}

string fold(string input, int n) {
  string output;

  for (int i = 0; i < n; ++i) {
    output += input;

    if (i != n - 1) {
      output += '?';
    }
  }

  return output;
}

vector<int> fold(const vector<int> &input, int n) {
  vector<int> output;

  for (int i = 0; i < n; ++i) {
    output.insert(output.end(), input.begin(), input.end());
  }

  return output;
}

int main(int argc, char *argv[]) {
  vector<string> lines = read_lines(argv[1]);
  long p1 = 0, p2 = 0;

  for (string line : lines) {
    vector<string> parts = split_space(line);
    string sequence = parts[0];
    vector<int> amounts = filter_map(split(parts[1], ","), parse_int);

    p1 += count(amounts, sequence);

    sequence = fold(sequence, 5);
    amounts = fold(amounts, 5);
    p2 += count(amounts, sequence);
  }

  assert_print(p1, p2, 7007L, 3476169006222L);
}
