#include "misc.h"
#include <string>

using namespace std;

optional<int> digit_first_last(const int ints) {
  string str = to_string(ints);
  string new_str;
  new_str += str[0];
  new_str += str[str.length() - 1];

  return stoi(new_str);
}

optional<int> char_first_last(const string input) {
  vector<string> digits = {"zero", "one", "two",   "three", "four",
                           "five", "six", "seven", "eight", "nine"};

  size_t first_pos = string::npos, last_pos = 0;
  string first_match, last_match;

  for (int i = 0; i < 10; ++i) {
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
  vector<string> lines = get_lines(argv[1]);

  vector<int> ints_p1 = transform_vector(lines, parse_digits);
  ints_p1 = transform_vector(ints_p1, digit_first_last);
  int p1 = sum(ints_p1);

  vector<int> ints_p2 = transform_vector(lines, char_first_last);
  int p2 = sum(ints_p2);

  assert_cout(p1, p2, 55090, 54845);
}
