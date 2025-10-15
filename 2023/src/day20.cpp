#include "misc/io.h"
#include "misc/string.h"
#include <queue>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

using Module = tuple<char, vector<string>, bool, unordered_map<string, bool>>;
using Modules = unordered_map<string, Module>;

Modules parse_modules(string path) {
  Modules modules;
  vector<string> lines = read_lines(path);
  for (string line : lines) {
    vector<string> parts = split(line, " -> ");
    char type = parts[0][0];

    string module = parts[0];
    module = regex_replace(module, regex("%"), "");
    module = regex_replace(module, regex("&"), "");
    vector<string> connections = split_commaspace(parts[1]);

    modules[module] = {type, connections, false, {}};
  }
  for (auto [module, meta] : modules) {
    auto [type, connections, on, inputs] = meta;
    for (string connection : connections) {
      auto &[contype, _, _, coninputs] = modules[connection];
      if (contype == '&') {
        coninputs[module] = false;
      }
    }
  }

  return modules;
}

queue<tuple<string, bool, string>> push_button(Modules &modules,
                                               const string sendto,
                                               const bool high, long &ls,
                                               long &hs, const string &sender) {

  auto &[type, outputs, on, inputs] = modules[sendto];
  bool send_high;
  bool send_at_all = true;

  // print(sender);
  // print(" -");
  // print(high?"high":"low");
  // print("-> ");
  // println(sendto);

  queue<tuple<string, bool, string>> tosend;

  if (type == 'b') {
    send_high = high;
  } else if (type == '%') {
    if (high) {
      send_at_all = false;
    } else {
      send_high = on ? false : true;
      get<2>(modules[sendto]) ^= true;
      // on = true;
    }
  } else {
    inputs[sender] = high;
    send_high = false;
    // println(" ");
    for (auto [m, sent_high] : inputs) {
      // println(m);
      if (!sent_high) {
        send_high = true;
      }
    }
    // println(" ");
  }

  if (send_at_all) {
    for (string output : outputs) {
      send_high ? ++hs : ++ls;
      tosend.emplace(output, send_high, sendto);
    }
  }
  return tosend;
}

int main(int argc, char *argv[]) {
  Modules modules = parse_modules(argv[1]);

  long ls = 0, hs = 0;
  for (int i = 0; i < stoi(argv[2]); ++i) {
    auto queue = push_button(modules, "broadcaster", false, ls, hs, "button");
    ++ls;
    while (!queue.empty()) {
      auto [soutput, ssend_high, ssender] = queue.front();
      queue.pop();
      auto newqueue =
          push_button(modules, soutput, ssend_high, ls, hs, ssender);
      while (!newqueue.empty()) {
        queue.push(newqueue.front());
        newqueue.pop();
      }
    }
  }

  // println(ls);
  // println(hs);
  println(ls * hs);
  // }
}
