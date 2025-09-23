#include "misc.h"
#include <fstream>
#include <optional>
#include <string>
#include <vector>

using namespace std;

vector<string> get_lines(string path) {
  vector<string> lines;
  string line;
  ifstream file(path);

  if (file.is_open()) {
    while (getline(file, line)) {
      lines.push_back(line);
    }
    file.close();
  }

  return lines;
}

optional<int> parse_digits(string input) {
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
