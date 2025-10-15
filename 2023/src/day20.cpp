#include "misc/io.h"
#include "misc/string.h"
#include <queue>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

using Module =
    tuple<char, bool, unordered_set<string>, unordered_map<string, bool>>;
using Modules = unordered_map<string, Module>;
using Queue = queue<tuple<string, bool, string>>;

Modules parse_modules(string path) {
  Modules modules;

  for (string line : read_lines(path)) {
    vector<string> line_parts = split(line, " -> ");

    string module = regex_replace(line_parts[0], regex("%|&"), "");
    char type = line_parts[0][0];
    bool state = false;
    vector<string> outputs_vec = split_commaspace(line_parts[1]);
    unordered_set<string> outputs(outputs_vec.begin(), outputs_vec.end());
    unordered_map<string, bool> inputs;

    modules[module] = {type, state, outputs, inputs};
  }

  for (auto [module, data] : modules) {
    auto [_, _, outputs, _] = data;
    for (string output : outputs) {
      auto &[_, _, _, inputs] = modules[output];
      inputs[module] = false;
    }
  }

  return modules;
}

Queue zap(Modules &modules, string sender, string receiver, bool high_pulse,
          int &lows, int &highs) {
  auto &[type, status, outputs, inputs] = modules[receiver];
  Queue to_zap;

  switch (type) {
  case '%':
    if (high_pulse) {
      return to_zap;
    }

    status ^= true;
    high_pulse = status;
    break;
  case '&':
    inputs[sender] = high_pulse;
    high_pulse = any_of(inputs.begin(), inputs.end(),
                        [](auto module) { return !module.second; });
    break;
  }

  for (string output : outputs) {
    high_pulse ? ++highs : ++lows;
    to_zap.emplace(output, high_pulse, receiver);
  }

  return to_zap;
}

int main(int argc, char *argv[]) {
  Modules modules = parse_modules(argv[1]);

  int iterations = 1000;
  int lows = iterations;
  int highs = 0;

  for (int i = 0; i < iterations; ++i) {
    Queue queue = zap(modules, "button", "broadcaster", false, lows, highs);

    while (!queue.empty()) {
      auto [receiver, high_pulse, sender] = queue.front();
      queue.pop();

      Queue queue_new = zap(modules, sender, receiver, high_pulse, lows, highs);
      while (!queue_new.empty()) {
        queue.push(queue_new.front());
        queue_new.pop();
      }
    }
  }

  int p1 = lows * highs;
  println(p1);
}
