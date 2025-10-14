#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <algorithm>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using Workflow = vector<tuple<char, bool, int, string>>;
using Workflows = unordered_map<string, Workflow>;
using Rating = vector<long>;
using Ratings = vector<Rating>;

Workflows parse_workflows(string input) {
  Workflows workflows;

  for (string workflow : split_newline(input)) {
    vector<string> workflow_parts = split(workflow, "{");
    string key = workflow_parts[0];
    string instructions = workflow_parts[1];
    instructions.pop_back();

    for (string instruction : split_comma(instructions)) {
      vector<string> instruction_parts = split(instruction, ":");

      if (instruction_parts.size() == 1) {
        workflows[key].emplace_back(' ', false, -1, instruction);
        break;
      }

      string equation = instruction_parts[0];
      string destination = instruction_parts[1];

      char argument = equation[0];
      bool op = equation[1] == '<';
      int numbers = parse_int(equation).value();

      workflows[key].emplace_back(argument, op, numbers, destination);
    }
  }

  return workflows;
}

Ratings parse_ratings(string input) {
  Ratings ratings;

  for (string rating : split_newline(input)) {
    vector<string> rating_parts_str = split_comma(rating);
    Rating rating_parts = filter_map(rating_parts_str, parse_long);
    ratings.push_back(rating_parts);
  }

  return ratings;
}

pair<Workflows, Ratings> parse_instructions(string path) {
  string text = read_text(path);
  vector<string> text_parts = split_newnewline(text);
  Workflows workflows = parse_workflows(text_parts[0]);
  Ratings ratings = parse_ratings(text_parts[1]);

  return {workflows, ratings};
}

bool equation_result(char argument, bool op, int numbers,
                     const Rating &rating) {
  int index = strchr("xmas", argument) - "xmas";
  return op ? rating[index] < numbers : rating[index] > numbers;
}

bool is_valid(const Workflows &workflows, const Rating &rating) {
  string key = "in";

  while (true) {
    Workflow workflow = workflows.at(key);

    for (auto [argument, op, numbers, destination] : workflow) {
      if (numbers != -1 && !equation_result(argument, op, numbers, rating)) {
        continue;
      }

      if (destination == "A") {
        return true;
      }

      if (destination == "R") {
        return false;
      }

      key = destination;
      break;
    }
  }
}

long solve_p1(const Ratings &ratings, const Workflows &workflows) {
  long total = 0;
  for (auto rating : ratings) {
    bool valid = is_valid(workflows, rating);
    if (valid) {
      total += sum(rating);
    }
  }

  return total;
}

long solve_p2(const Ratings &ratings, const Workflows &workflows) {
  long total = 0;

  vector<int> sx = {0};
  vector<int> sm = {0};
  vector<int> sa = {0};
  vector<int> ss = {0};

  for (auto [_, workflow] : workflows) {
    for (auto [c, op, in, _] : workflow) {
      switch (c) {
      case 'x':
        sx.push_back(op ? in - 1 : in);
        sx.push_back(op ? in - 1 : in);
        break;
      case 'm':
        sm.push_back(op ? in - 1 : in);
        sm.push_back(op ? in - 1 : in);
        break;
      case 'a':
        sa.push_back(op ? in - 1 : in);
        sa.push_back(op ? in - 1 : in);
        break;
      case 's':
        ss.push_back(op ? in - 1 : in);
        ss.push_back(op ? in - 1 : in);
        break;
      }
    }
  }

  sx.push_back(4000);
  sm.push_back(4000);
  sa.push_back(4000);
  ss.push_back(4000);

  sort(sx.begin(), sx.end());
  sort(sm.begin(), sm.end());
  sort(sa.begin(), sa.end());
  sort(ss.begin(), ss.end());

  for (int xi = 0; xi < sx.size(); xi += 2) {
    for (int mi = 0; mi < sm.size(); mi += 2) {
      for (int ai = 0; ai < sa.size(); ai += 2) {
        for (int si = 0; si < ss.size(); si += 2) {

          Rating rating_last = {sx[xi + 0], sm[mi + 0], sa[ai + 0], ss[si + 0]};
          Rating rating = {sx[xi + 1], sm[mi + 1], sa[ai + 1], ss[si + 1]};

          if (is_valid(workflows, rating)) {
            total += ((rating.at(0) - rating_last.at(0)) *
                      (rating.at(1) - rating_last.at(1)) *
                      (rating.at(2) - rating_last.at(2)) *
                      (rating.at(3) - rating_last.at(3)));
          }
        }
      }
    }
  }

  return total;
}

int main(int argc, char *argv[]) {
  auto [workflows, ratings] = parse_instructions(argv[1]);

  long p1 = solve_p1(ratings, workflows);
  long p2 = solve_p2(ratings, workflows);

  assert_print(p1, p2, 19114L, 167409079868000L);
}
