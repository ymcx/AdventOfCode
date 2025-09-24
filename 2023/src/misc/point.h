#pragma once

#include <cstddef>
#include <functional>
#include <ostream>

using namespace std;

struct Point {
  int y, x;

  Point() : y(0), x(0) {}
  Point(int y, int x) : y(y), x(x) {}

  bool operator==(const Point &p) const { return y == p.y && x == p.x; }

  bool operator!=(const Point &p) const { return y != p.y || x != p.x; }

  Point operator+(const Point &p) const { return Point(y + p.y, x + p.x); }

  Point operator-(const Point &p) const { return Point(y - p.y, x - p.x); }

  friend ostream &operator<<(ostream &os, const Point &p) {
    os << "(" << p.y << ", " << p.x << ")";
    return os;
  }
};

namespace std {
template <> struct hash<Point> {
  size_t operator()(const Point &p) const {
    size_t h1 = hash<int>{}(p.y);
    size_t h2 = hash<int>{}(p.x);
    return h1 ^ (h2 << 1);
  }
};
} // namespace std
