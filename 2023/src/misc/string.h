#pragma once

#include <optional>
#include <string>
#include <vector>

using namespace std;

string parse(string input);

optional<int> parse_int(string input);

optional<long> parse_long(string input);

optional<long long> parse_long_long(string input);

optional<float> parse_float(string input);

optional<double> parse_double(string input);

vector<string> split(string input, string delimiter);

vector<string> split_comma(string input);

vector<string> split_commaspace(string input);

vector<string> split_space(string input);

vector<string> split_newline(string input);

vector<string> split_newnewline(string input);
