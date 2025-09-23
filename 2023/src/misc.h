#pragma once

#include <cassert>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

using namespace std;

vector<string> get_lines(string path);

optional<int> parse_digits(string input);

vector<string> split(string input, string delimiter);

vector<string> split_list(string input);

vector<string> split_space(string input);

template <typename T, typename F>
vector<F> transform_vector(const vector<T> &input, optional<F> function(T)) {
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

template <typename T> T sum(const vector<T> &input) {
  return accumulate(input.begin(), input.end(), 0);
}

template <typename T> void print(T input) { cout << input; }

template <typename T> void println(T input) {
  print(input);
  print("\n");
}

template <typename T, typename F>
void assert_cout(T p1, F p2, T res_p1, F res_p2) {
  println(p1);
  assert(p1 == res_p1);
  println(p2);
  assert(p2 == res_p2);
}
