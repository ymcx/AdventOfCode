#include "misc/io.h"
#include "misc/range.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

pair<vector<long>, vector<unordered_map<long, Range>>>
parse_seeds(string path) {
  vector<string> lines = read_lines(path);
  string seeds_str = regex_replace(lines[0], regex(".+: "), "");
  vector<string> seeds_vec = split_space(seeds_str);
  vector<long> seeds = filter_map(seeds_vec, parse_long);

  vector<unordered_map<long, Range>> maps;
  for (string line : lines) {
    if (line.contains(':')) {
      continue;
    }

    if (line.empty()) {
      maps.emplace_back();
      continue;
    }

    istringstream stream(line);
    long dst, src, len;
    stream >> dst >> src >> len;
    maps.back()[src] = Range(dst, len);
  }

  return {seeds, maps};
}

Ranges parse_ranges(const vector<long> &seeds) {
  vector<Range> seed_ranges;

  for (int i = 0; i < seeds.size() / 2; ++i) {
    long start = seeds[i * 2];
    long len = seeds[i * 2 + 1];
    seed_ranges.emplace_back(start, start + len);
  }

  return Ranges(seed_ranges);
}

long solve_p1(const vector<long> &seeds,
              const vector<unordered_map<long, Range>> &maps) {
  long lowest = numeric_limits<long>::max();

  for (long seed : seeds) {
    for (unordered_map<long, Range> map : maps) {
      for (auto [src, val] : map) {
        auto [dst, len] = val;
        if (seed >= src && seed < src + len) {
          seed += dst - src;
          break;
        }
      }
    }

    lowest = min(seed, lowest);
  }

  return lowest;
}

long solve_p2(Ranges ranges, const vector<unordered_map<long, Range>> &maps) {
  Ranges ranges_to_add;

  for (unordered_map<long, Range> map : maps) {
    for (auto [src, val] : map) {
      auto [dst, len] = val;
      Ranges mapping_range = Ranges({src, src + len});
      Ranges overlaps = mapping_range.intersection(ranges);

      for (Range overlap : overlaps) {
        Range range = Range(overlap.start + dst - src, overlap.end + dst - src);
        ranges_to_add = ranges_to_add.union_with(range);
      }

      ranges = ranges.difference(overlaps);
    }

    ranges = ranges.union_with(ranges_to_add);
    ranges_to_add = Ranges();
  }

  return ranges.first();
}

int main(int argc, char *argv[]) {
  auto [seeds, maps] = parse_seeds(argv[1]);
  Ranges ranges = parse_ranges(seeds);

  int p1 = solve_p1(seeds, maps);
  int p2 = solve_p2(ranges, maps);

  assert_print(p1, p2, 484023871, 46294175);
}
