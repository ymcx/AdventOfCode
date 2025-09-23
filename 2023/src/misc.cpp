#include "misc.h"
#include <fstream>
#include <optional>
#include <string>
#include <vector>

std::vector<std::string> get_lines(std::string path) {
  std::vector<std::string> lines;
  std::string line;
  std::ifstream file(path);

  if (file.is_open()) {
    while (getline(file, line)) {
      lines.push_back(line);
    }
    file.close();
  }

  return lines;
}

std::optional<int> parse_digits(std::string input) {
  std::string digits = "";

  for (char c : input) {
    if (std::isdigit(c)) {
      digits += c;
    }
  }

  if (digits.empty()) {
    return std::nullopt;
  }

  return stoi(digits);
}
