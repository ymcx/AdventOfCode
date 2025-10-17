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
  bool collides_with(const brick &other) const {
    for (int x = start[0]; x <= end[0]; ++x) {
      for (int y = start[1]; y <= end[1]; ++y) {
        for (int z = start[2]; z <= end[2]; ++z) {
          for (int x2 = other.start[0]; x2 <= other.end[0]; ++x2) {
            for (int y2 = other.start[1]; y2 <= other.end[1]; ++y2) {
              for (int z2 = other.start[2]; z2 <= other.end[2]; ++z2) {
                if (x == x2 && y == y2 && z == z2) {
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
    size_t h1 = hash<int>{}(b.start[0]);
    size_t h2 = hash<int>{}(b.start[1]);
    size_t h3 = hash<int>{}(b.start[2]);
    size_t h4 = hash<int>{}(b.end[0]);
    size_t h5 = hash<int>{}(b.end[1]);
    size_t h6 = hash<int>{}(b.end[2]);
    return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4) ^ (h6 << 5);
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
  int can_drop_to(const brick &br) {
    for (int new_z = br.start[2] - 1; new_z >= 1; --new_z) {
      const brick new_br = brick(br, new_z);

      for (const brick &old_br : all_bricks) {
        if (old_br == br) {
          continue;
        }

        if (old_br.collides_with(new_br)) {
          return new_z + 1;
        }
      }
    }

    return 1;
  }

  // Drop all bricks in the list to the lowest level
  void drop_all() {
    for (int i = 0; i < all_bricks.size(); ++i) {
      const brick &br = all_bricks.at(i);
      const int new_z = can_drop_to(br);

      all_bricks[i] = brick(br, new_z);
    }
  }

  // Get amount of bricks that can be removed
  int can_remove_amount() {
    int count = 0;

    for (int i = 0; i < all_bricks.size(); ++i) {
      bricks brs1 = *this;

      brs1.all_bricks.erase(brs1.all_bricks.begin() + i);
      bricks brs2 = brs1;

      brs1.drop_all();

      if (brs1.all_bricks == brs2.all_bricks) {
        ++count;
      }
    }

    return count;
  }
};

void test1() {
  brick b = brick("1,2,3~4,5,6");

  assert((b.start == array<int, 3>{1, 2, 3}));
  assert((b.end == array<int, 3>{4, 5, 6}));
}

void test2() {
  vector<brick> vec;
  vec.push_back(brick("1,2,3~4,5,6"));
  vec.push_back(brick("5,5,5~3,3,3"));

  bricks bs = bricks(vector<string>{"1,2,3~4,5,6", "5,5,5~3,3,3"});

  assert((bs.all_bricks == vec));
}

void test3() {
  brick b1 = brick("0,0,1~0,0,2");
  brick b2 = brick("0,0,2~0,0,2");

  brick b3 = brick("0,0,1~0,0,2");
  brick b4 = brick("0,0,3~0,0,3");

  brick b5 = brick("1,0,1~1,3,1");
  brick b6 = brick("0,1,1~3,1,1");

  brick b7 = brick("1,0,1~1,3,1");
  brick b8 = brick("2,1,1~3,1,1");

  assert((b1.collides_with(b2) == true));
  assert((b3.collides_with(b4) == false));
  assert((b5.collides_with(b6) == true));
  assert((b7.collides_with(b8) == false));
}

void test4() {
  brick b1 = brick("0,0,8~0,3,8");
  brick b2 = brick(b1, 2);

  assert((b2 == brick("0,0,2~0,3,2")));
}

void test5() {
  bricks bs = bricks(vector<string>{"0,0,5~1,0,5"});
  bs.drop_all();
  brick first = bs.all_bricks.at(0);

  assert((first.start == array<int, 3>{0, 0, 1}));
  assert((first.end == array<int, 3>{1, 0, 1}));
}

void test6() {
  bricks bs = bricks(vector<string>{"0,0,5~3,0,5", "0,0,8~0,3,8"});
  bs.drop_all();
  brick first = bs.all_bricks.at(0);
  brick second = bs.all_bricks.at(1);

  assert((first.start == array<int, 3>{0, 0, 1}));
  assert((first.end == array<int, 3>{3, 0, 1}));

  assert((second.start == array<int, 3>{0, 0, 2}));
  assert((second.end == array<int, 3>{0, 3, 2}));
}

void test7() {
  bricks bs = bricks(vector<string>{"0,0,8~0,3,8", "0,0,5~3,0,5"});
  bs.drop_all();
  brick first = bs.all_bricks.at(0);
  brick second = bs.all_bricks.at(1);

  assert((first.start == array<int, 3>{0, 0, 6}));
  assert((first.end == array<int, 3>{0, 3, 6}));

  assert((second.start == array<int, 3>{0, 0, 1}));
  assert((second.end == array<int, 3>{3, 0, 1}));
}

void test8() {
  vector<string> lines1 = {"0,0,1~0,0,1", "1,1,1~1,1,1", "1,0,1~1,0,1",
                           "0,1,1~0,1,1"};
  bricks bs1 = bricks(lines1);
  bs1.drop_all();
  int amount1 = bs1.can_remove_amount();

  vector<string> lines2 = {"0,0,1~0,0,1", "0,0,2~0,0,2"};
  bricks bs2 = bricks(lines2);
  bs2.drop_all();
  int amount2 = bs2.can_remove_amount();

  vector<string> lines3 = {"0,1,1~4,1,1", "0,2,1~4,2,1", "1,0,2~1,3,2"};
  bricks bs3 = bricks(lines3);
  bs3.drop_all();
  int amount3 = bs3.can_remove_amount();

  vector<string> lines4 = {"0,1,1~4,1,1", "1,0,2~1,3,2"};
  bricks bs4 = bricks(lines4);
  bs4.drop_all();
  int amount4 = bs4.can_remove_amount();

  vector<string> lines5 = {"1,0,1~1,2,1", "0,0,2~2,0,2", "0,2,2~2,2,2",
                           "0,0,3~0,2,3", "2,0,3~2,2,3", "0,1,4~2,1,4",
                           "1,1,5~1,1,5"};
  bricks bs5 = bricks(lines5);
  bs5.drop_all();
  int amount5 = bs5.can_remove_amount();

  assert((amount1 == 4));
  assert((amount2 == 1));
  assert((amount3 == 3));
  assert((amount4 == 1));
  assert((amount5 == 5));
}

int main(const int argc, const char *argv[]) {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();

  const vector<string> lines = read_lines(argv[1]);
  bricks all_bricks = bricks(lines);
  all_bricks.drop_all();
  const int amount = all_bricks.can_remove_amount();
  println(amount);
}
