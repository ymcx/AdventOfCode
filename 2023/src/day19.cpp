#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <algorithm>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using Workflow = vector<tuple<char, bool, long, string>>;
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
      long numbers = parse_long(equation).value();

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

bool equation_result(char argument, bool op, long numbers,
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

long solve_p1(const Workflows &workflows, const Ratings &ratings) {
  long result = 0;

  for (Rating rating : ratings) {
    if (is_valid(workflows, rating)) {
      result += sum(rating);
    }
  }

  return result;
}

unordered_map<char, Rating> collect_rating_borders(const Workflows &workflows) {
  unordered_map<char, Rating> borders = {
      {'x', {0}}, {'m', {0}}, {'a', {0}}, {'s', {0}}};

  for (auto [_, workflow] : workflows) {
    for (auto [argument, op, numbers, _] : workflow) {
      if (numbers == -1) {
        continue;
      }

      long value = op ? numbers - 1 : numbers;
      borders[argument].push_back(value);
    }
  }

  for (auto &[_, borders] : borders) {
    sort(borders.begin(), borders.end());
    borders.push_back(4000);
  }

  return borders;
}

long solve_p2(const Workflows &workflows, const Ratings &ratings) {
  unordered_map<char, Rating> borders = collect_rating_borders(workflows);
  long result = 0;

  for (int x = 0; x < borders['x'].size() - 1; ++x) {
    long x0 = borders['x'][x];
    long x1 = borders['x'][x + 1];

    for (int m = 0; m < borders['m'].size() - 1; ++m) {
      long m0 = borders['m'][m];
      long m1 = borders['m'][m + 1];

      for (int a = 0; a < borders['a'].size() - 1; ++a) {
        long a0 = borders['a'][a];
        long a1 = borders['a'][a + 1];

        for (int s = 0; s < borders['s'].size() - 1; ++s) {
          long s0 = borders['s'][s];
          long s1 = borders['s'][s + 1];

          Rating rating = {x1, m1, a1, s1};
          if (is_valid(workflows, rating)) {
            result += (x1 - x0) * (m1 - m0) * (a1 - a0) * (s1 - s0);
          }
        }
      }
    }
  }

  return result;
}

int main(int argc, char *argv[]) {
  auto [workflows, ratings] = parse_instructions(argv[1]);

  long p1 = solve_p1(workflows, ratings);
  long p2 = solve_p2(workflows, ratings);

  assert_print(p1, p2, 19114L, 167409079868000L);
}
