#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <string>
#include <vector>

using namespace std;

vector<vector<string>> parse_mirrors(string path) {
  string text = read_text(path);
  vector<string> mirrors = split_newnewline(text);
  return map(mirrors, split_newline);
}

vector<string> transpose(const vector<string> &input) {
  vector<string> output;
  int max_y = input.size();
  int max_x = input[0].size();

  for (int x = 0; x < max_x; ++x) {
    string line;
    for (int y = 0; y < max_y; ++y) {
      line += input[y][x];
    }

    output.push_back(line);
  }

  return output;
}

bool differ(string a, string b, int &changes, bool p2) {
  for (int i = 0; i < a.length(); ++i) {
    if (a[i] != b[i]) {
      ++changes;
    }
  }

  return ((!p2 && changes > 0) || (p2 && changes > 1));
}

int reflection(const vector<string> &mirrors, bool p2) {
  for (int i = 0, j = 1; j < mirrors.size(); ++i, ++j) {
    int changes = 0;
    if (differ(mirrors[i], mirrors[j], changes, p2)) {
      continue;
    }

    for (int a = i - 1, b = j + 1; true; --a, ++b) {
      if (a < 0 || b >= mirrors.size()) {
        if ((!p2 && changes == 0) || (p2 && changes == 1)) {
          return j;
        }

        break;
      }

      if (differ(mirrors[a], mirrors[b], changes, p2)) {
        break;
      }
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {
  int p1 = 0, p2 = 0;

  for (vector<string> mirrors : parse_mirrors(argv[1])) {
    p1 += reflection(mirrors, false) * 100;
    p2 += reflection(mirrors, true) * 100;
    mirrors = transpose(mirrors);
    p1 += reflection(mirrors, false);
    p2 += reflection(mirrors, true);
  }

  assert_print(p1, p2, 35691, 39037);
}
