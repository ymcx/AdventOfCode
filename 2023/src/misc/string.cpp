#include "string.h"
#include <optional>
#include <string>
#include <vector>

using namespace std;

optional<int> parse(string input) {
  string digits = "";

  for (char c : input) {
    if (isdigit(c)) {
      digits += c;
    }
  }

  if (digits.empty()) {
    return nullopt;
  }

  return stoi(digits);
}

vector<string> split(string input, string delimiter) {
  vector<string> tokens;
  size_t start = 0, end;

  while ((end = input.find(delimiter, start)) != string::npos) {
    tokens.push_back(input.substr(start, end - start));
    start = end + delimiter.length();
  }
  tokens.push_back(input.substr(start));

  return tokens;
}

vector<string> split_list(string input) { return split(input, ", "); }

vector<string> split_space(string input) { return split(input, " "); }
