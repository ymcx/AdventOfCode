#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <regex>
#include <string>
#include <vector>

using namespace std;

pair<vector<long>, vector<vector<tuple<long, long, long>>>>
parse_seeds(string path) {
  vector<string> lines = read_lines(path);
  string seeds_str = regex_replace(lines[0], regex(".+: "), "");
  vector<string> seeds_vec = split(seeds_str, " ");
  vector<long> seeds = filter_map(seeds_vec, parse_long);

  int i = -1;
  vector<vector<tuple<long, long, long>>> maps;
  maps.resize(7);

  for (string line : lines) {
    if (line.empty()) {
      ++i;
      continue;
    }

    if (line.contains(':')) {
      continue;
    }

    vector<string> tokens = split(line, " ");
    vector<long> values = filter_map(tokens, parse_long);
    maps[i].emplace_back(values[0], values[1], values[2]);
  }

  return {seeds, maps};
}

vector<pair<long, long>> parse_ranges(const vector<long> &seeds) {
  vector<pair<long, long>> seed_ranges;
  for (int i = 0; i < seeds.size() / 2; ++i) {
    long start = seeds[i * 2];
    long len = seeds[i * 2 + 1];
    seed_ranges.emplace_back(start, start + len);
  }

  return seed_ranges;
}

long reverse_map(long location,
                 const vector<vector<tuple<long, long, long>>> &maps) {
  for (int i = maps.size() - 1; i >= 0; --i) {
    for (auto [dst, src, len] : maps[i]) {
      if (dst <= location && location < dst + len) {
        location += src - dst;
        break;
      }
    }
  }

  return location;
}

bool is_seed(long seed, const vector<pair<long, long>> &seed_ranges) {
  for (auto [min, max] : seed_ranges) {
    if (min <= seed && seed < max) {
      return true;
    }
  }

  return false;
}

int main(int argc, char *argv[]) {
  auto [seeds, maps] = parse_seeds(argv[1]);
  vector<pair<long, long>> seed_ranges = parse_ranges(seeds);
  long seed = 0, location = 0, p1 = 0, p2 = 0;

  while (!p1 || !p2) {
    seed = reverse_map(location, maps);

    if (!p1 && contains(seeds, seed)) {
      p1 = location;
    }

    if (!p2 && is_seed(seed, seed_ranges)) {
      p2 = location;
    }

    ++location;
  }

  assert_print(p1, p2, 484023871, 46294175);
}
