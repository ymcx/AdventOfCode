#pragma once

#include <cstddef>
#include <functional>
#include <ostream>

constexpr int RIGHT = 0;
constexpr int DOWN = 1;
constexpr int LEFT = 2;
constexpr int UP = 3;

struct Point {
  int y, x;

  constexpr Point() : y(0), x(0) {}
  constexpr Point(int y, int x) : y(y), x(x) {}

  constexpr bool operator==(const Point &p) const {
    return y == p.y && x == p.x;
  }

  constexpr bool operator!=(const Point &p) const { return !(*this == p); }

  constexpr Point operator+(const Point &p) const {
    return Point(y + p.y, x + p.x);
  }

  constexpr Point operator-(const Point &p) const {
    return Point(y - p.y, x - p.x);
  }

  friend std::ostream &operator<<(std::ostream &os, const Point &p) {
    os << "(" << p.y << ", " << p.x << ")";
    return os;
  }

  constexpr bool is_valid(const Point &dimensions) const {
    return y >= 0 && x >= 0 && y < dimensions.y && x < dimensions.x;
  }
};

struct DirectionalPoint {
  int y, x, direction;

  constexpr DirectionalPoint() : y(0), x(0), direction(0) {}
  constexpr DirectionalPoint(int y, int x, int direction)
      : y(y), x(x), direction(direction) {}

  constexpr bool operator==(const DirectionalPoint &p) const {
    return y == p.y && x == p.x && direction == p.direction;
  }

  constexpr bool operator!=(const DirectionalPoint &p) const {
    return !(*this == p);
  }

  constexpr DirectionalPoint operator+(const DirectionalPoint &p) const {
    return DirectionalPoint(y + p.y, x + p.x, direction);
  }

  constexpr DirectionalPoint operator-(const DirectionalPoint &p) const {
    return DirectionalPoint(y - p.y, x - p.x, direction);
  }

  friend std::ostream &operator<<(std::ostream &os, const DirectionalPoint &p) {
    os << "(" << p.y << ", " << p.x << ", " << p.direction << ")";
    return os;
  }

  constexpr void move_inplace(const int new_direction) {
    direction = new_direction;
    switch (direction) {
    case RIGHT:
      x += 1;
      break;
    case DOWN:
      y += 1;
      break;
    case LEFT:
      x -= 1;
      break;
    case UP:
      y -= 1;
      break;
    }
  }

  constexpr DirectionalPoint move(const int new_direction) const {
    DirectionalPoint new_point = *this;
    new_point.move_inplace(new_direction);
    return new_point;
  }

  constexpr bool is_valid(const Point &dimensions) const {
    return y >= 0 && x >= 0 && y < dimensions.y && x < dimensions.x &&
           direction >= 0 && direction <= 3;
  }

  constexpr Point get_position() const { return Point(y, x); }
};

namespace std {
template <> struct hash<Point> {
  constexpr size_t operator()(const Point &p) const {
    size_t h1 = hash<int>{}(p.y);
    size_t h2 = hash<int>{}(p.x);
    return h1 ^ (h2 << 1);
  }
};

template <> struct hash<DirectionalPoint> {
  constexpr size_t operator()(const DirectionalPoint &p) const {
    size_t h1 = hash<int>{}(p.y);
    size_t h2 = hash<int>{}(p.x);
    size_t h3 = hash<int>{}(p.direction);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};
} // namespace std
