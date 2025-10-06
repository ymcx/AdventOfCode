#include "misc/io.h"
#include "misc/string.h"
#include <regex>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  vector<string> lines = read_lines(argv[1]);
  int p1 = 0, p2 = 0;

  for (auto [i, line] : views::enumerate(lines)) {
    map<string, int> needed = {{"red", 0}, {"green", 0}, {"blue", 0}};
    line = regex_replace(line, regex(".+: "), "");
    line = regex_replace(line, regex(";"), ",");

    vector<string> balls = split_commaspace(line);
    for (string ball : balls) {
      vector<string> ball_parts = split_space(ball);
      int amount = stoi(ball_parts[0]);
      string color = ball_parts[1];

      if (needed[color] < amount) {
        needed[color] = amount;
      }
    }

    if (needed["red"] <= 12 && needed["green"] <= 13 && needed["blue"] <= 14) {
      p1 += i + 1;
    }

    p2 += needed["red"] * needed["green"] * needed["blue"];
  }

  assert_print(p1, p2, 3035, 66027);
}
