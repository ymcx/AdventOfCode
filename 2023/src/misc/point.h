#pragma once

#include <cstddef>
#include <functional>
#include <ostream>

using namespace std;

constexpr int RIGHT = 0;
constexpr int DOWN = 1;
constexpr int LEFT = 2;
constexpr int UP = 3;

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

  bool is_valid(Point dimensions) {
    return y >= 0 && x >= 0 && y < dimensions.y && x < dimensions.x;
  }
};

struct Point3D {
  int y, x, z;

  Point3D() : y(0), x(0), z(0) {}
  Point3D(int y, int x, int z) : y(y), x(x), z(z) {}

  bool operator==(const Point3D &p) const {
    return x == p.x && y == p.y && z == p.z;
  }

  bool operator!=(const Point3D &p) const {
    return x != p.x || y != p.y || z != p.z;
  }

  Point3D operator+(const Point3D &p) const {
    return Point3D(x + p.x, y + p.y, z + p.z);
  }

  Point3D operator-(const Point3D &p) const {
    return Point3D(z - p.z, y - p.y, z - p.z);
  }

  friend ostream &operator<<(ostream &os, const Point3D &p) {
    os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return os;
  }

  bool is_valid(Point3D dimensions) {
    return x >= 0 && y >= 0 && z >= 0 && x < dimensions.x && y < dimensions.y &&
           z < dimensions.z;
  }
};

struct DirectionalPoint {
  int y, x, direction;

  DirectionalPoint() : y(0), x(0), direction(0) {}
  DirectionalPoint(int y, int x, int direction)
      : y(y), x(x), direction(direction) {}

  bool operator==(const DirectionalPoint &p) const {
    return y == p.y && x == p.x && direction == p.direction;
  }

  bool operator!=(const DirectionalPoint &p) const {
    return y != p.y || x != p.x || direction != p.direction;
  }

  DirectionalPoint operator+(const DirectionalPoint &p) const {
    return DirectionalPoint(y + p.y, x + p.x, direction);
  }

  DirectionalPoint operator-(const DirectionalPoint &p) const {
    return DirectionalPoint(y - p.y, x - p.x, direction);
  }

  friend ostream &operator<<(ostream &os, const DirectionalPoint &p) {
    os << "(" << p.y << ", " << p.x << ", " << p.direction << ")";
    return os;
  }

  void move_inplace(int new_direction) {
    direction = new_direction;
    switch (direction) {
    case (RIGHT):
      x += 1;
      break;
    case (DOWN):
      y += 1;
      break;
    case (LEFT):
      x -= 1;
      break;
    case (UP):
      y -= 1;
      break;
    }
  }

  DirectionalPoint move(int new_direction) {
    DirectionalPoint new_point = *this;
    new_point.move_inplace(new_direction);
    return new_point;
  }

  bool is_valid(Point dimensions) {
    return y >= 0 && x >= 0 && y < dimensions.y && x < dimensions.x &&
           0 <= direction && direction <= 3;
  }

  Point get_position() { return Point(this->y, this->x); }
};

namespace std {
template <> struct hash<Point> {
  size_t operator()(const Point &p) const {
    size_t h1 = hash<int>{}(p.y);
    size_t h2 = hash<int>{}(p.x);
    return h1 ^ (h2 << 1);
  }
};

template <> struct hash<Point3D> {
  size_t operator()(const Point3D &p) const {
    size_t h1 = hash<int>{}(p.y);
    size_t h2 = hash<int>{}(p.x);
    size_t h3 = hash<int>{}(p.z);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};

template <> struct hash<DirectionalPoint> {
  size_t operator()(const DirectionalPoint &p) const {
    size_t h1 = hash<int>{}(p.y);
    size_t h2 = hash<int>{}(p.x);
    size_t h3 = hash<int>{}(p.direction);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};
} // namespace std
