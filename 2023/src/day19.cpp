#include "misc/io.h"
#include "misc/string.h"
#include "misc/vector.h"
#include <boost/numeric/interval.hpp>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

using namespace boost::numeric;
using namespace std;

using Workflow = vector<tuple<char, bool, long, string>>;
using Workflows = unordered_map<string, Workflow>;
using Rating = vector<long>;
using Ratings = vector<Rating>;
using Interval = interval<int>;
using Intervals = vector<Interval>;

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

// https://github.com/iglesias/coding-challenges
Intervals filter_intervals(const Intervals &intervals, int pivot,
                           bool greater_than, bool restricted) {
  Intervals output;
  Interval filter =
      restricted
          ? (greater_than ? interval(1, pivot - 1) : interval(pivot + 1, 4000))
          : (greater_than ? interval(pivot, 4000) : interval(1, pivot));

  for (Interval interval : intervals) {
    interval = intersect(interval, filter);
    output.push_back(interval);
  }

  return output;
}

long sum(const unordered_map<char, Intervals> &intervals) {
  Interval x = intervals.at('x').at(0);
  Interval m = intervals.at('m').at(0);
  Interval a = intervals.at('a').at(0);
  Interval s = intervals.at('s').at(0);

  long xl = x.lower();
  long ml = m.lower();
  long al = a.lower();
  long sl = s.lower();

  long xu = x.upper();
  long mu = m.upper();
  long au = a.upper();
  long su = s.upper();

  return (xu - xl + 1) * (mu - ml + 1) * (au - al + 1) * (su - sl + 1);
}

long apply(string destination, unordered_map<char, Intervals> intervals,
           const Workflows &workflows, long result) {
  if (destination == "A") {
    return result + sum(intervals);
  }

  if (destination == "R") {
    return result;
  }

  Workflow workflow = workflows.at(destination);
  for (auto [argument, op, numbers, destination] : workflow) {
    if (numbers == -1) {
      return apply(destination, intervals, workflows, result);
    }

    unordered_map<char, Intervals> intervals_restricted = intervals;
    intervals_restricted[argument] =
        filter_intervals(intervals[argument], numbers, op, true);
    intervals[argument] =
        filter_intervals(intervals[argument], numbers, op, false);

    result = apply(destination, intervals_restricted, workflows, result);
  }

  return result;
}

long solve_p2(const Workflows &workflows) {
  unordered_map<char, Intervals> intervals = {
      {'x', {{1, 4000}}},
      {'m', {{1, 4000}}},
      {'a', {{1, 4000}}},
      {'s', {{1, 4000}}},
  };

  return apply("in", intervals, workflows, 0);
}

int main(int argc, char *argv[]) {
  auto [workflows, ratings] = parse_instructions(argv[1]);

  long p1 = solve_p1(workflows, ratings);
  long p2 = solve_p2(workflows);

  assert_print(p1, p2, 389114L, 125051049836302L);
}
