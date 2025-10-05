#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> read_lines(string path);

string read_text(string path);

void println();

template <typename T> void print(T input) { cout << input; }

template <typename T> void print(vector<T> input) {
  print("[");
  for (int i = 0; i < input.size() - 1; ++i) {
    print(input[i]);
    print(", ");
  }
  print(input.back());
  print("]");
}

template <typename T> void println(T input) {
  print(input);
  print("\n");
}

template <typename T, typename F>
void assert_print(T p1, F p2, T res_p1, F res_p2) {
  println(p1);
  assert(p1 == res_p1);
  println(p2);
  assert(p2 == res_p2);
}
