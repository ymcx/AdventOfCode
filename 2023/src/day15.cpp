#include "misc/io.h"
#include "misc/string.h"
#include <string>
#include <vector>

using namespace std;

void code(int &total, const char &c) {
  total += int(c);
  total *= 17;
  total %= 256;
}

int main(int argc, char *argv[]) {
  vector<string> parts = split_comma(read_text(argv[1]));
  int total = 0;
  for (const string &part : parts) {
    int some = 0;
    for (const char &c : part) {
      code(some, c);
    }
    total += some;
  }
  println(total);
}
