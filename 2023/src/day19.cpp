#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

pair<unordered_map<string, vector<tuple<char, bool, int, string>>>,
     vector<vector<int>>>
parse_instructions(string path) {
  string text = read_text(path);
  vector<string> parts = split_newnewline(text);

  unordered_map<string, vector<tuple<char, bool, int, string>>> workflows;
  for (string workflow : split_newline(parts[0])) {
    vector<string> workflow_parts = split(workflow, "{");
    workflow = workflow_parts[0];
    string instructions =
        workflow_parts[1].substr(0, workflow_parts[1].length() - 1);

    for (string instruction : split_comma(instructions)) {
      vector<string> p = split(instruction, ":");
      if (p.size() == 1) {
        workflows[workflow].push_back({' ', false, -1, p[0]});
        break;
      } else {
        char letter = p[0][0];
        bool op = p[0][1] == '<';
        int nums = parse_int(p[0]).value();
        string destionation = p[1];
        workflows[workflow].push_back({letter, op, nums, destionation});
      }
    }
  }

  vector<vector<int>> ratings;
  for (string rating : split_newline(parts[1])) {
    vector<string> rating_parts = split_comma(rating);
    int a = stoi(rating_parts[0].substr(3));
    int b = stoi(rating_parts[1].substr(2));
    int c = stoi(rating_parts[2].substr(2));
    int d = stoi(rating_parts[3].substr(2, rating_parts[3].size() - 3));
    ratings.push_back({a, b, c, d});
  }

  return {workflows, ratings};
}

bool get_statement(char letter, bool op, int nums, int x, int m, int a, int s) {
  switch (letter) {
  case 'x':
    return op ? x < nums : x > nums;
  case 'm':
    return op ? m < nums : m > nums;
  case 'a':
    return op ? a < nums : a > nums;
  case 's':
    return op ? s < nums : s > nums;
  default:
    return false;
  }
}

int is_valid(const vector<int> &rating,
             const unordered_map<string, vector<tuple<char, bool, int, string>>>
                 &workflows) {
  int x = rating[0];
  int m = rating[1];
  int a = rating[2];
  int s = rating[3];

  string index = "in";
  while (true) {
    vector<tuple<char, bool, int, string>> workflow = workflows.at(index);
    for (tuple<char, bool, int, string> w : workflow) {
      auto [letter, op, nums, dest] = w;

      if (nums == -1) {
        if (dest == "A") {
          return true;
        } else if (dest == "R") {
          return false;
        }
        index = dest;
        break;
      }

      bool statement = get_statement(letter, op, nums, x, m, a, s);

      if (statement) {
        if (dest == "A") {
          return true;
        } else if (dest == "R") {
          return false;
        }
        index = dest;
        break;
      }
    }
  }
}

int solve_p1(const vector<vector<int>> &ratings,
             const unordered_map<string, vector<tuple<char, bool, int, string>>>
                 &workflows) {
  int total = 0;
  for (vector<int> rating : ratings) {
    bool valid = is_valid(rating, workflows);
    if (valid) {
      total += sum(rating);
    }
  }

  return total;
}

int main(int argc, char *argv[]) {
  auto [workflows, ratings] = parse_instructions(argv[1]);
  int p1 = solve_p1(ratings, workflows);
  println(p1);
}
