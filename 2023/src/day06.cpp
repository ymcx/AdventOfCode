#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <string>
#include <vector>

using namespace std;

vector<pair<long, long>> parse_races(const vector<string> &lines) {
  vector<long> times = filter_map(split_space(lines[0]), parse_long);
  vector<long> bests = filter_map(split_space(lines[1]), parse_long);

  vector<pair<long, long>> races;
  races.reserve(times.size());

  for (int i = 0; i < times.size(); ++i) {
    races.emplace_back(times[i], bests[i]);
  }

  return races;
}

pair<long, long> parse_race(const vector<string> &lines) {
  long time = parse_long(lines[0]).value();
  long best = parse_long(lines[1]).value();

  return {time, best};
}

int win_amount(long time, long best) {
  int amount = 0;

  for (int hold = 1; hold < time; ++hold) {
    if (best < (time - hold) * hold) {
      ++amount;
    }
  }

  return amount;
}

int main(int argc, char *argv[]) {
  vector<string> lines = read_lines(argv[1]);

  int p1 = 1;
  for (auto [time, best] : parse_races(lines)) {
    p1 *= win_amount(time, best);
  }

  auto [time, best] = parse_race(lines);
  int p2 = win_amount(time, best);

  assert_print(p1, p2, 211904, 43364472);
}
