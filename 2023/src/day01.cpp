#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <string>
#include <vector>

using namespace std;

int digit_first_last(int ints) {
  string str = to_string(ints);
  string new_str;
  new_str += str[0];
  new_str += str[str.length() - 1];

  return stoi(new_str);
}

int char_first_last(string input) {
  vector<string> digits = {"zero", "one", "two",   "three", "four",
                           "five", "six", "seven", "eight", "nine"};

  size_t first_pos = string::npos, last_pos = 0;
  string first_match, last_match;

  for (int i = 0; i < digits.size(); ++i) {
    for (string token : {digits[i], to_string(i)}) {
      size_t pos = input.find(token);
      if (pos != string::npos && pos < first_pos) {
        first_pos = pos;
        first_match = to_string(i);
      }

      pos = input.rfind(token);
      if (pos != string::npos && pos > last_pos) {
        last_pos = pos;
        last_match = to_string(i);
      }
    }
  }

  if (last_match.empty()) {
    last_match = first_match;
  }

  return stoi(first_match + last_match);
}

int main(int argc, char *argv[]) {
  vector<string> lines = read_lines(argv[1]);

  vector<int> ints_p1 = filter_map(lines, parse_int);
  ints_p1 = map(ints_p1, digit_first_last);
  int p1 = sum(ints_p1);

  vector<int> ints_p2 = map(lines, char_first_last);
  int p2 = sum(ints_p2);

  assert_print(p1, p2, 55090, 54845);
}
