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

string read_text(string path) {
  string text;
  string line;
  ifstream file(path);

  if (file.is_open()) {
    while (getline(file, line)) {
      text += line;
      text += '\n';
    }
    file.close();
  }

  return text.substr(0, text.size()-1);
}

void println() { print("\n"); }
