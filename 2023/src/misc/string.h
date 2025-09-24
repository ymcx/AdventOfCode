#pragma once

#include <optional>
#include <string>
#include <vector>

using namespace std;

optional<int> parse(string input);

vector<string> split(string input, string delimiter);

vector<string> split_list(string input);

vector<string> split_space(string input);
