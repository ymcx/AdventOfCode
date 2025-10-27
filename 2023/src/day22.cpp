#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <cassert>
#include <string>
#include <vector>

using namespace std;

struct brick {
  array<int, 3> start;
  array<int, 3> end;

  unordered_set<brick *> supported_by;
  unordered_set<brick *> supports;

  // Construct from one line from input
  brick(const string &line) {
    const vector<string> pair = split(line, "~");

    const vector<int> start = filter_map(split_comma(pair[0]), parse_int);
    const vector<int> end = filter_map(split_comma(pair[1]), parse_int);

    this->start = {start[0], start[1], start[2]};
    this->end = {end[0], end[1], end[2]};
  }

  // Construct from old brick and a given Z level
  brick(const brick &br, const int &new_z) {
    start = br.start;
    end = br.end;

    int delta = start[2] - new_z;

    start[2] -= delta;
    end[2] -= delta;
  }

  // Checks if X and Y collides with another brick,
  // Z is only checked for the lowest value
  bool collides_with(const brick &other, int elevation) const {
    for (int x = start[0]; x <= end[0]; ++x) {
      for (int y = start[1]; y <= end[1]; ++y) {
        for (int z = start[2]; z <= end[2]; ++z) {
          for (int x2 = other.start[0]; x2 <= other.end[0]; ++x2) {
            for (int y2 = other.start[1]; y2 <= other.end[1]; ++y2) {
              for (int z2 = other.start[2]; z2 <= other.end[2]; ++z2) {
                if (x == x2 && y == y2 && z == z2 - elevation) {
                  return true;
                }
              }
            }
          }
        }
      }
    }

    return false;
  }

  bool operator==(const brick &other) const {
    return start == other.start && end == other.end;
  }
};

namespace std {
template <> struct hash<brick> {
  size_t operator()(const brick &b) const {
    return hash<size_t>{}(b.start[0]) ^ hash<size_t>{}(b.start[1]) ^
           hash<size_t>{}(b.start[2]) ^ hash<size_t>{}(b.end[0]) ^
           hash<size_t>{}(b.end[1]) ^ hash<size_t>{}(b.end[2]);
  }
};
} // namespace std

struct bricks {
  vector<brick> all_bricks;

  // Construct from all lines from input
  bricks(const vector<string> &lines) {
    for (const string &line : lines) {
      const brick one_brick = brick(line);
      all_bricks.push_back(one_brick);
    }
  }

  // Get the lowest Z to which the brick can drop to
  brick drop(const brick &br) {
    for (int new_z = br.start[2] - 1; new_z >= 1; --new_z) {
      const brick new_br = brick(br, new_z);

      for (const brick &old_br : all_bricks) {
        if (old_br == br) {
          continue;
        }

        if (old_br.collides_with(new_br, 0)) {
          return brick(br, new_z + 1);
        }
      }
    }

    return brick(br, 1);
  }

  // Drop all bricks in the list to the lowest level
  void drop_all() {
    for (int i = 0; i < all_bricks.size(); ++i) {
      const brick &br = all_bricks.at(i);
      all_bricks[i] = drop(br);
    }
  }

  // Add bricks that support/are supported by a brick
  void add_supports() {
    for (int i = 0; i < all_bricks.size(); ++i) {
      for (int j = 0; j < all_bricks.size(); ++j) {
        if (i == j) {
          continue;
        }

        auto &br1 = all_bricks.at(i);
        auto &br2 = all_bricks.at(j);
        if (br1.collides_with(br2, 1)) {
          br1.supports.insert(&br2);
          br2.supported_by.insert(&br1);
        }
      }
    }
  }

  // Get amount of bricks that can be removed
  int can_remove_amount() {
    int count = 0;

    for (auto br : all_bricks) {
      bool safet = true;

      for (auto thisissupported : br.supports) {

        if (thisissupported->supported_by.size() == 1) {
          safet = false;
        }
      }

      if (!safet) {
        count += 1;
      }
    }

    return all_bricks.size() - count;
  }
};

int main(const int argc, const char *argv[]) {
  const vector<string> lines = read_lines(argv[1]);
  bricks all_bricks = bricks(lines);
  bricks old_bricks = all_bricks;
  do {
    old_bricks = all_bricks;
    all_bricks.drop_all();
  } while (all_bricks.all_bricks != old_bricks.all_bricks);
  all_bricks.add_supports();
  const int amount = all_bricks.can_remove_amount();
  println(amount);
}
