#pragma once

#include "point.h"
#include <numeric>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

unordered_set<Point> coordinates(const vector<string> &board,
                                 const vector<char> &targets);

template <typename T, typename F>
vector<F> map(const vector<T> &input, F function(T)) {
  vector<F> output;
  output.reserve(input.size());

  for (T i : input) {
    F res = function(i);
    output.push_back(res);
  }

  return output;
}

template <typename T> int sum(const T &input) {
  return accumulate(input.begin(), input.end(), 0);
}

template <typename T, typename F>
vector<F> filter_map(const vector<T> &input, optional<F> function(T)) {
  vector<F> output;
  output.reserve(input.size());

  for (T i : input) {
    optional<F> res = function(i);
    if (res.has_value()) {
      output.push_back(res.value());
    }
  }

  return output;
}

template <typename T, typename F, typename R>
vector<T> filter(const vector<T> &input, F function(T)) {
  vector<T> output;
  output.reserve(input.size());

  for (T i : input) {
    F res = function(i);
    if (is_same_v<F, bool> && res) {
      output.push_back(i);
    } else if (is_same_v<F, optional<R>> && res.has_value()) {
      output.push_back(i);
    }
  }

  return output;
}

template <typename T, typename F>
void each(const vector<T> &input, F function(T)) {
  for (T i : input) {
    function(i);
  }
}

template <typename T> bool contains(const vector<T> &vec, T value) {
  return find(vec.begin(), vec.end(), value) != vec.end();
}
