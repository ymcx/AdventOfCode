#pragma once

#include <cstddef>
#include <functional>

using namespace std;

struct Point {
  int y, x;

  Point(int y, int x) : y(y), x(x) {}

  bool operator==(const Point &p) const { return x == p.x && y == p.y; }
};

struct PointHash {
  size_t operator()(const Point &p) const {
    return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
  }
};
