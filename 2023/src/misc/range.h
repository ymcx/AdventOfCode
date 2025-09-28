#pragma once

#include <algorithm>
#include <vector>

using namespace std;

struct Range {
  long start;
  long end;

  bool operator<(const Range &other) { return start < other.start; }
};

struct Ranges {
  vector<Range> ranges;

  Ranges() {}
  Ranges(const Range &other) { ranges = {other}; }
  Ranges(const vector<Range> &other) { ranges = other; }

  Ranges union_with(const vector<Range> &other) {
    ranges.insert(ranges.end(), other.begin(), other.end());
    sort(ranges.begin(), ranges.end());

    vector<Range> results;
    Range current = ranges[0];

    for (int i = 1; i < ranges.size(); ++i) {
      const Range &next = ranges[i];
      if (current.end < next.start) {
        results.push_back(current);
        current = next;
      } else {
        current.end = max(current.end, next.end);
      }
    }

    results.push_back(current);
    return Ranges(results);
  }

  Ranges union_with(const Ranges &other) { return union_with(other.ranges); }

  Ranges intersection(const vector<Range> &other) {
    vector<Range> results;

    for (const Range &a : other) {
      for (const Range &b : ranges) {
        if (a.start < b.end && a.end > b.start) {
          long start = max(a.start, b.start);
          long end = min(a.end, b.end);
          results.emplace_back(start, end);
        }
      }
    }

    return Ranges(results);
  }

  Ranges intersection(const Ranges &other) {
    return intersection(other.ranges);
  }

  Ranges difference(const vector<Range> &other) {
    vector<Range> results = ranges;

    for (const Range &a : other) {
      vector<Range> new_results;
      for (const Range &b : results) {
        if (b.start >= a.end || b.end <= a.start) {
          new_results.push_back(b);
          continue;
        }
        if (b.start < a.start) {
          new_results.emplace_back(b.start, a.start);
        }
        if (b.end > a.end) {
          new_results.emplace_back(a.end, b.end);
        }
      }
      results = new_results;
    }

    return Ranges(results);
  }

  Ranges difference(const Ranges &other) { return difference(other.ranges); }

  long first() { return ranges.front().start; }

  vector<Range>::const_iterator begin() { return ranges.begin(); }

  vector<Range>::const_iterator end() { return ranges.end(); }
};
