#include "string.h"
#include <optional>
#include <string>
#include <vector>

using namespace std;

string parse(string input) {
  string digits = "";

  for (char c : input) {
    if (isdigit(c) || c == '-') {
      digits += c;
    }
  }

  return digits;
}

optional<int> parse_int(string input) {
  string output = parse(input);
  if (output.empty()) {
    return nullopt;
  }

  return stoi(output);
}

optional<long> parse_long(string input) {
  string output = parse(input);
  if (output.empty()) {
    return nullopt;
  }

  return stol(output);
}

optional<long long> parse_long_long(string input) {
  string output = parse(input);
  if (output.empty()) {
    return nullopt;
  }

  return stoll(output);
}

optional<float> parse_float(string input) {
  optional<long> output = parse_long(input);

  return output.transform([](long i) { return static_cast<float>(i); });
}

optional<double> parse_double(string input) {
  optional<long> output = parse_long(input);

  return output.transform([](long i) { return static_cast<double>(i); });
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

vector<string> split_comma(string input) { return split(input, ","); }

vector<string> split_commaspace(string input) { return split(input, ", "); }

vector<string> split_colonspace(string input) { return split(input, ": "); }

vector<string> split_space(string input) { return split(input, " "); }

vector<string> split_newline(string input) { return split(input, "\n"); }

vector<string> split_newnewline(string input) { return split(input, "\n\n"); }
