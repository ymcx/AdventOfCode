#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <cmath>
#include <regex>
#include <string>
#include <vector>

using namespace std;

pair<vector<int>, vector<int>> cards(string line) {
  line = regex_replace(line, regex(".+: "), "");
  vector<string> parts = split(line, " | ");
  vector<int> cards_l = filter_map(split_space(parts[0]), parse);
  vector<int> cards_r = filter_map(split_space(parts[1]), parse);

  return {cards_l, cards_r};
}

vector<int> winning_amount(const vector<string> &lines) {
  vector<int> total_amount;

  for (string line : lines) {
    int amount = 0;
    auto [cards_l, cards_r] = cards(line);
    for (int card_r : cards_r) {
      if (contains(cards_l, card_r)) {
        ++amount;
      }
    }
    total_amount.push_back(amount);
  }

  return total_amount;
}

optional<int> get_points(int amount) {
  if (amount > 0) {
    return pow(2, amount - 1);
  }

  return nullopt;
}

int main(int argc, char *argv[]) {
  vector<string> lines = read_lines(argv[1]);
  vector<int> amount = winning_amount(lines);
  vector<int> points = filter_map(amount, get_points);
  int p1 = sum(points);
  println(p1);
}
