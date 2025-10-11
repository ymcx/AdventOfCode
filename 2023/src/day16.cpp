#include "misc/io.h"
#include "misc/point.h"
#include <queue>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

pair<unordered_map<Point, char>, Point> parse_board(string path) {
  vector<string> lines = read_lines(path);
  unordered_map<Point, char> board;
  int max_y = lines.size();
  int max_x = lines[0].size();
  Point dimensions = Point(max_y, max_x);
  for (const auto [y, line] : views::enumerate(lines)) {
    for (const auto [x, item] : views::enumerate(line)) {
      Point point = Point(y, x);
      switch (item) {
      case ('|'):
      case ('-'):
      case ('/'):
      case ('\\'):
        board[point] = item;
        break;
      }
    }
  }
  return {board, dimensions};
}

// const int RIGHT = 0;
// const int DOWN = 1;
// const int LEFT = 2;
// const int UP = 3;

// DirectionalPoint move_to(DirectionalPoint player, int direction) {
//   switch (direction) {
//   case (RIGHT):
//     player.x += 1;
//     break;
//   case (DOWN):
//     player.y += 1;
//     break;
//   case (LEFT):
//     player.x -= 1;
//     break;
//   case (UP):
//     player.y -= 1;
//     break;
//   }
//   player.direction = direction;
//   return player;
// }

int main(int argc, char *argv[]) {
  auto [board, dimensions] = parse_board(argv[1]);
  unordered_set<DirectionalPoint> been;
  queue<DirectionalPoint> to_search;
  to_search.push(DirectionalPoint(0, 0, RIGHT));
  while (!to_search.empty()) {
    auto player = to_search.front();
    to_search.pop();
    // println(position);

    if (player.x < 0 || player.y < 0 || player.x >= dimensions.x ||
        player.y >= dimensions.y || been.contains(player)) {
      continue;
    }
    been.insert(player);

    if (board.contains({player.y, player.x})) {
      switch (board[{player.y, player.x}]) {
      case ('|'):
        if (player.direction == UP || player.direction == DOWN) {
          to_search.push(player.move(player.direction));
        } else {
          to_search.push(player.move(UP));
          to_search.push(player.move(DOWN));
        }
        break;
      case ('-'):
        if (player.direction == RIGHT || player.direction == LEFT) {
          to_search.push(player.move(player.direction));
        } else {
          to_search.push(player.move(RIGHT));
          to_search.push(player.move(LEFT));
        }
        break;
      case ('/'):
        if (player.direction == RIGHT) {
          to_search.push(player.move(UP));
        }
        if (player.direction == DOWN) {
          to_search.push(player.move(LEFT));
        }
        if (player.direction == UP) {
          to_search.push(player.move(RIGHT));
        }
        if (player.direction == LEFT) {
          to_search.push(player.move(DOWN));
        }
        break;
      case ('\\'):
        if (player.direction == RIGHT) {
          to_search.push(player.move(DOWN));
        }
        if (player.direction == DOWN) {
          to_search.push(player.move(RIGHT));
        }
        if (player.direction == UP) {
          to_search.push(player.move(LEFT));
        }
        if (player.direction == LEFT) {
          to_search.push(player.move(UP));
        }

        break;
      }
      continue;
    }

    to_search.push(player.move(player.direction));
  }
  unordered_set<Point> been2;
  for (DirectionalPoint p : been) {
    been2.insert({p.y, p.x});
  }
  println(been2.size());
}
