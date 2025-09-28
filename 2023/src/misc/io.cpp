#include "io.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> read_lines(string path) {
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

void println() { print("\n"); }
