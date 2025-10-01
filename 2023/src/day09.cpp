#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <algorithm>

using namespace std;

int main(int argc, char *argv[]) {
  vector<string> lines = read_lines(argv[1]);
  int p1 = 0;
  int p2 = 0;

  for (string line : lines) {
    unordered_map<int, vector<int>> layers;
    layers[0] = filter_map(split(line, " "), parse_int);

    for (int l = 0; true; ++l) {
      vector<int> layer;

      for (int i = 1; i < layers[l].size(); ++i) {
        int cur = layers[l][i];
        int prev = layers[l][i - 1];
        layer.push_back(cur - prev);
      }

      if (ranges::all_of(layer, [](int i) { return i == 0; })) {
        for (int i = 0; i < layers.size(); ++i) {
          p1 += layers[i].back();
          p2 += layers[i].front() * (i % 2 == 0 ? 1 : -1);
        }

        break;
      }

      layers[l + 1] = layer;
    }
  }

  assert_print(p1, p2, 1762065988, 1066);
}
