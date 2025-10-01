#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"

using namespace std;

int main(int argc, char *argv[]) {
  vector<string> lines = read_lines(argv[1]);
  int result = 0;

  for (string line : lines) {

    unordered_map<int, vector<int>> layers;
    layers[0] = filter_map(split(line, " "), parse_int);

    for (int layer = 0; true; ++layer) {
      vector<int> new_layer;
      for (int i = 1; i < layers[layer].size(); ++i) {
        int value = layers[layer][i];
        int last = layers[layer][i - 1];
        int new_value = value - last;
        new_layer.push_back(new_value);
      }

      if (all_of(new_layer.begin(), new_layer.end(),
                 [](int i) { return i == 0; })) {
        for (auto [_, v] : layers) {
          result += v.back();
        }
        break;
      }

      layers[layer + 1] = new_layer;
    }
  }

  println(result);
}
