#include "misc/io.h"
#include "misc/string.h"
#include <string>
#include <vector>

using namespace std;

const int get_code(const string &label) {
  int code = 0;
  for (const char &c : label) {
    code = ((code + int(c)) * 17) % 256;
  }
  return code;
}

const int find(const vector<pair<string, int>> &box, const string &label) {
  for (int n = 0; n < box.size(); ++n) {
    if (box[n].first == label) {
      return n;
    }
  }
  return -1;
}

const int get_p2(const vector<vector<pair<string, int>>> &boxes) {
  int p2 = 0;
  for (int i = 0; i < boxes.size(); ++i) {
    for (int j = 0; j < boxes[i].size(); ++j) {
      p2 += (i + 1) * (j + 1) * boxes[i][j].second;
    }
  }
  return p2;
}

const pair<int, int> solve(const string &path) {
  vector<vector<pair<string, int>>> boxes(256);
  int p1 = 0, p2 = 0;

  const vector<string> list = split_comma(read_text(path));
  for (const string &item : list) {
    p1 += get_code(item);

    const bool is_minus = item.back() == '-';
    const string label = item.substr(0, item.length() - (is_minus ? 1 : 2));
    const int code = get_code(label);
    vector<pair<string, int>> &box = boxes[code];
    const int n = find(box, label);

    if (is_minus) {
      if (n != -1) {
        box.erase(box.begin() + n);
      }
    } else {
      const int focal = item.back() - '0';
      if (n != -1) {
        box[n].second = focal;
      } else {
        box.emplace_back(label, focal);
      }
    }
  }

  p2 = get_p2(boxes);
  return {p1, p2};
}

int main(const int argc, const char *argv[]) {
  const auto [p1, p2] = solve(argv[1]);
  assert_print(p1, p2, 503487, 261505);
}
