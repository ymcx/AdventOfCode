#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <cassert>
#include <queue>
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
  bool collides_with(const brick &other, const bool &is_below) const {
    for (int x0 = start[0]; x0 <= end[0]; ++x0) {
      for (int y0 = start[1]; y0 <= end[1]; ++y0) {
        for (int z0 = start[2]; z0 <= end[2]; ++z0) {
          for (int x1 = other.start[0]; x1 <= other.end[0]; ++x1) {
            for (int y1 = other.start[1]; y1 <= other.end[1]; ++y1) {
              for (int z1 = other.start[2]; z1 <= other.end[2]; ++z1) {
                const bool x_matches = x0 == x1;
                const bool y_matches = y0 == y1;
                const bool z_matches = z0 == z1 - is_below ? 1 : 0;

                if (x_matches && y_matches && z_matches) {
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
  size_t operator()(const brick &br) const {
    const size_t a = hash<int>{}(br.start[0]);
    const size_t b = hash<int>{}(br.start[1]);
    const size_t c = hash<int>{}(br.start[2]);
    const size_t d = hash<int>{}(br.end[0]);
    const size_t e = hash<int>{}(br.end[1]);
    const size_t f = hash<int>{}(br.end[2]);
    return (a << 0) ^ (b << 1) ^ (c << 2) ^ (d << 3) ^ (e << 4) ^ (f << 5);
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
    for (int z = br.start[2]; z > 1; --z) {
      const brick new_br = brick(br, z);

      for (const brick &old_br : all_bricks) {
        if (old_br == br) {
          continue;
        }

        if (old_br.collides_with(new_br, true)) {
          return brick(br, z);
        }
      }
    }

    return brick(br, 1);
  }

  // Drop all bricks in the list to the lowest level
  void drop_all() {
    bool changed = true;

    while (changed) {
      changed = false;

      for (brick &br : all_bricks) {
        const brick dropped = drop(br);
        if (br != dropped) {
          br = dropped;
          changed = true;
        }
      }
    }
  }

  // Add bricks that support/are supported by a brick
  void add_supports() {
    for (int i = 0; i < all_bricks.size(); ++i) {
      for (int j = 0; j < all_bricks.size(); ++j) {
        if (i == j) {
          continue;
        }

        brick &a = all_bricks.at(i);
        brick &b = all_bricks.at(j);

        if (a.collides_with(b, true)) {
          // b rests on top of a
          a.supports.insert(&b);
          b.supported_by.insert(&a);
        }
      }
    }
  }

  // Get amount of bricks that can be removed
  int can_remove_amount() {
    int cant_remove = 0;

    for (const brick &br : all_bricks) {
      for (const brick *supported : br.supports) {
        if (supported->supported_by.size() != 1) {
          // brick is supported by multiple bricks, br could be removed
          continue;
        }

        // brick is supported by just one brick, br can't be removed
        ++cant_remove;
        break;
      }
    }

    return all_bricks.size() - cant_remove;
  }

  // Get the amount of bricks that would fall if br was to be removed
  int would_fall_amount(const brick &br) {
    unordered_set<brick> fallen({br});
    queue<brick> to_travel;

    // Visit all bricks that br supports
    for (const brick *supported : br.supports) {
      to_travel.push(*supported);
    }

    while (!to_travel.empty()) {
      const brick br = to_travel.front();
      to_travel.pop();

      // Every brick that supported br has fallen
      bool all_supporters_fallen = true;
      for (const brick *supports : br.supported_by) {
        if (!fallen.contains(*supports)) {
          all_supporters_fallen = false;
        };
      }

      // If so, add br to the list of fallen
      // Also visit all bricks that br supports
      if (all_supporters_fallen) {
        fallen.insert(br);

        for (const brick *supported : br.supports) {
          to_travel.push(*supported);
        }
      }
    }

    return fallen.size();
  }

  // Get the amount of bricks that would fall
  // if every brick would be removed one by one
  int would_fall_amount() {
    int amount = 0;

    for (const brick &br : all_bricks) {
      amount += would_fall_amount(br);
    }

    return amount - all_bricks.size();
  }
};

int main(const int argc, const char *argv[]) {
  const vector<string> lines = read_lines(argv[1]);
  bricks all_bricks = bricks(lines);

  all_bricks.drop_all();
  all_bricks.add_supports();

  const int p1 = all_bricks.can_remove_amount();
  const int p2 = all_bricks.would_fall_amount();

  assert_print(p1, p2, 490, 96356);
}
