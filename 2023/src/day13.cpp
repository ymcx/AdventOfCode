#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <vector>

using namespace std;

vector<vector<string>> parse_walls(string path) {
  string text = read_text(path);
  vector<string> boards = split(text, "\n\n");
  return map(boards, split_newline);
}

vector<string> transpose(vector<string> input) {
  int rows = input.size();
  int cols = input[0].size();

  vector<string> output;

  for (int col = 0; col < cols; ++col) {
    string l;
    for (int row = rows - 1; row >= 0; --row) {
      l += input[row][col];
    }
    output.push_back(l);
  }

  return output;
}

int isunique(vector<string> walls) {
  int retiisi = 0;

  for (int i = 0; i < walls.size() - 1; ++i) {
    int j = i + 1;

    if (walls[i] == walls[j]) {
      retiisi = j;

      int a = i;
      int b = j;
      while (true) {
        if (a < 0 || b >= walls.size()) {
          return retiisi;
        }
        if (walls[a] != walls[b]) {
          break;
        }
        ++b;
        --a;
      }
    }
  }
  return -1;
}

int main(int argc, char *argv[]) {
  int total = 0;
  vector<vector<string>> all_walls = parse_walls(argv[1]);
  for (vector<string> walls : all_walls) {
    bool passed = false;
    int ref = isunique(walls);
    if (ref != -1) {
      passed = true;
      total += ref * 100;
    }

    walls = transpose(walls);
    ref = isunique(walls);
    if (ref != -1) {
      passed = true;
      total += ref * 1;
    }
  }
  println(total);
}
