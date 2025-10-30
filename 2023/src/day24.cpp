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

array<long, 6> parse_line(const string input) {
  const vector<string> splitted = split_space(input);
  const vector<long> parsed = filter_map(splitted, parse_long);

  array<long, 6> output;
  copy_n(parsed.data(), 6, output.begin());

  return output;
}

int solve_p1(const vector<array<long, 6>> &lines) {
  int intersections = 0;

  for (int i = 0; i < lines.size(); ++i) {
    for (int j = i + 1; j < lines.size(); ++j) {
      const auto &[x0, y0, _, x0d, y0d, _] = lines[i];
      const auto &[x1, y1, _, x1d, y1d, _] = lines[j];

      const Vector2f p0(x0, y0);
      const Vector2f p1(x0 + static_cast<float>(x0d),
                        y0 + static_cast<float>(y0d));
      const Hyperplane p = Hyperplane<float, 2>::Through(p0, p1);

      const Vector2f q0(x1, y1);
      const Vector2f q1(x1 + static_cast<float>(x1d),
                        y1 + static_cast<float>(y1d));
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

  return intersections;
}

// https://www.reddit.com/user/mynt
// https://pastebin.com/pnbxaCVu
long solve_p2(const vector<array<long, 6>> &lines) {
  const auto &[x0, y0, z0, vx0, vy0, vz0] = lines[72];
  const auto &[x1, y1, z1, vx1, vy1, vz1] = lines[101];
  const auto &[x2, y2, z2, vx2, vy2, vz2] = lines[189];

  const __int128 vxr1 = vx1 - vx0;
  const __int128 vzr1 = vz1 - vz0;
  const __int128 vxr2 = vx2 - vx0;
  const __int128 vzr2 = vz2 - vz0;

  const __int128 xr1 = x1 - x0;
  const __int128 yr1 = y1 - y0;
  const __int128 zr1 = z1 - z0;

  const __int128 xr2 = x2 - x0;
  const __int128 yr2 = y2 - y0;
  const __int128 zr2 = z2 - z0;

  __int128 num =
      yr2 * (xr1 * vzr1 - vxr1 * zr1) + yr1 * (zr2 * vxr1 - vzr1 * xr2);

  __int128 den = yr1 * (vzr1 * vxr2 - vxr1 * vzr2);
  const __int128 t2 = num / den;

  num = yr1 * (xr2 + vxr2 * t2) - yr2 * xr1;
  den = yr2 * vxr1;
  const __int128 t1 = num / den;

  const __int128 cx1 = x1 + t1 * vx1;
  const __int128 cy1 = y1 + t1 * vy1;
  const __int128 cz1 = z1 + t1 * vz1;

  const __int128 cx2 = x2 + t2 * vx2;
  const __int128 cy2 = y2 + t2 * vy2;
  const __int128 cz2 = z2 + t2 * vz2;

  const __int128 xm = (cx2 - cx1) / (t2 - t1);
  const __int128 ym = (cy2 - cy1) / (t2 - t1);
  const __int128 zm = (cz2 - cz1) / (t2 - t1);

  const __int128 xc = cx1 - xm * t1;
  const __int128 yc = cy1 - ym * t1;
  const __int128 zc = cz1 - zm * t1;

  return xc + yc + zc;
}

int main(const int argc, const char *argv[]) {
  const vector<string> lines_str = read_lines(argv[1]);
  const vector<array<long, 6>> lines = map(lines_str, parse_line);

  const int p1 = solve_p1(lines);
  const long p2 = solve_p2(lines);

  assert_print(p1, p2, 15889, 801386475216902L);
}
