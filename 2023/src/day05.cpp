#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <regex>
#include <string>
#include <vector>

using namespace std;

tuple<vector<long>, vector<vector<tuple<long, long, long>>>>
parse_seeds(string path) {
  vector<string> lines = read_lines(path);
  string seeds_str = regex_replace(lines[0], regex(".+: "), "");
  vector<string> seeds_vec = split(seeds_str, " ");
  vector<long> seeds = filter_map(seeds_vec, parse_long);

  vector<vector<tuple<long, long, long>>> maps;
  maps.resize(40);
  long i = 0;
  for (long ln = 3; ln < lines.size(); ++ln) {
    string line = lines[ln];
    if (line.empty()) {
      continue;
    }
    if (line.contains('m')) {
      i += 1;
      continue;
    }
    vector<string> strings = split(line, " ");
    vector<long> nums = filter_map(strings, parse_long);
    tuple<long, long, long> t = {nums[0], nums[1], nums[2]};
    maps[i].push_back(t);
  }

  return {seeds, maps};
}

tuple<long, bool> convert(long input, tuple<long, long, long> range) {
  long dst = get<0>(range);
  long src = get<1>(range);
  long len = get<2>(range);

  if (input < src || src + len <= input) {
    return {input, false};
  }

  long delta = dst - src;
  return {input + delta, true};
}

int main(int argc, char *argv[]) {
  auto [seeds, maps] = parse_seeds(argv[1]);

  long smallest = -1;
  for (long seed : seeds) {
    for (auto map : maps) {
      for (tuple<long, long, long> t : map) {
        auto [newseed, converted] = convert(seed, t);
        if (converted) {
          seed = newseed;
          break;
        }
      }
    }
    if (smallest == -1) {
      smallest = seed;
    }
    if (seed < smallest) {
      smallest = seed;
    }
  }
  println(smallest);
}
