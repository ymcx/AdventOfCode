#pragma once

#include <cassert>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

std::vector<std::string> get_lines(std::string path);

std::optional<int> parse_digits(std::string input);

template <typename T, typename F>
std::vector<F> transform_vector(const std::vector<T> &input,
                                std::optional<F> function(T)) {
  std::vector<F> output;
  output.reserve(input.size());

  for (T i : input) {
    std::optional<F> res = function(i);
    if (res.has_value()) {
      output.push_back(res.value());
    }
  }

  return output;
}

template <typename T> T sum(const std::vector<T> &input) {
  return std::accumulate(input.begin(), input.end(), 0);
}

template <typename T> void print(T input) { std::cout << input; }

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
