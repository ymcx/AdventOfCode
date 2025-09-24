#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> read_lines(string path);

template <typename T> void print(T input) { cout << input; }

template <typename T> void println(T input) { cout << input << endl; }

template <typename T, typename F>
void assert_print(T p1, F p2, T res_p1, F res_p2) {
  println(p1);
  assert(p1 == res_p1);
  println(p2);
  assert(p2 == res_p2);
}
