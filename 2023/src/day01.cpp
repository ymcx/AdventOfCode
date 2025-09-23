#include "misc.h"
#include <string>

std::optional<int> digit_first_last(const int ints) {
  std::string str = std::to_string(ints);
  std::string new_str;
  new_str += str[0];
  new_str += str[str.length() - 1];

  return stoi(new_str);
}

std::optional<int> char_first_last(const std::string input) {
  std::vector<std::string> digits = {"zero", "one", "two",   "three", "four",
                                     "five", "six", "seven", "eight", "nine"};

  size_t first_pos = std::string::npos, last_pos = 0;
  std::string first_match, last_match;

  for (int i = 0; i < 10; ++i) {
    for (std::string token : {digits[i], std::to_string(i)}) {
      size_t pos = input.find(token);
      if (pos != std::string::npos && pos < first_pos) {
        first_pos = pos;
        first_match = std::to_string(i);
      }

      pos = input.rfind(token);
      if (pos != std::string::npos && pos > last_pos) {
        last_pos = pos;
        last_match = std::to_string(i);
      }
    }
  }

  if (last_match.empty()) {
    last_match = first_match;
  }

  return stoi(first_match + last_match);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> lines = get_lines(argv[1]);

  std::vector<int> ints_p1 = transform_vector(lines, parse_digits);
  ints_p1 = transform_vector(ints_p1, digit_first_last);
  int p1 = sum(ints_p1);

  std::vector<int> ints_p2 = transform_vector(lines, char_first_last);
  int p2 = sum(ints_p2);

  assert_cout(p1, p2, 55090, 54845);
}
