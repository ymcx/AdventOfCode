#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <Eigen/Geometry>
#include <string>
#include <vector>

using namespace std;
using Eigen::Hyperplane;
using Eigen::Vector2f;

constexpr float LOW = 200000000000000.0F;
constexpr float HIGH = 400000000000000.0F;

array<float, 6> parse_line(const string input) {
  const vector<string> splitted = split_space(input);
  const vector<float> parsed = filter_map(splitted, parse_float);

  array<float, 6> output;
  copy_n(parsed.data(), 6, output.begin());

  return output;
}

int main(const int argc, const char *argv[]) {
  const vector<string> lines_str = read_lines(argv[1]);
  const vector<array<float, 6>> lines = map(lines_str, parse_line);

  int intersections = 0;

  for (int i = 0; i < lines.size(); ++i) {
    for (int j = i + 1; j < lines.size(); ++j) {
      const auto &[x0, y0, _, x0d, y0d, _] = lines[i];
      const auto &[x1, y1, _, x1d, y1d, _] = lines[j];

      const Vector2f p0(x0, y0);
      const Vector2f p1(x0 + x0d, y0 + y0d);
      const Hyperplane p = Hyperplane<float, 2>::Through(p0, p1);

      const Vector2f q0(x1, y1);
      const Vector2f q1(x1 + x1d, y1 + y1d);
      const Hyperplane q = Hyperplane<float, 2>::Through(q0, q1);

      const auto intersection = p.intersection(q);
      const float ix = intersection.x();
      const float iy = intersection.y();

      // Out of bounds
      if (ix < LOW || HIGH < ix || iy < LOW || HIGH < iy) {
        continue;
      }

      const bool x0_facing_l = x0d < 0;
      const bool x1_facing_l = x1d < 0;

      const bool x0_on_l = x0 < ix;
      const bool x1_on_l = x1 < ix;

      const bool c1 = x0_facing_l && x1_facing_l && !x0_on_l && !x1_on_l;
      const bool c2 = x0_facing_l && !x1_facing_l && !x0_on_l && x1_on_l;
      const bool c3 = !x0_facing_l && !x1_facing_l && x0_on_l && x1_on_l;
      const bool c4 = !x0_facing_l && x1_facing_l && x0_on_l && !x1_on_l;

      // The intersection is located in the wrong direction
      if (!c1 && !c2 && !c3 && !c4) {
        continue;
      }

      ++intersections;
    }
  }

  println(intersections);
}
