use crate::misc;
use regex::Regex;

fn strip_numbers(string: &str) -> usize {
    string
        .chars()
        .filter(|c| c.is_ascii_digit())
        .collect::<String>()
        .parse()
        .unwrap()
}

pub fn a() -> usize {
    let re = Regex::new(r"mul\(\d{1,3},\d{1,3}\)").unwrap();
    let text = misc::text();

    re.find_iter(&text)
        .map(|result| {
            let (l, r) = result.as_str().split_once(",").unwrap();
            strip_numbers(l) * strip_numbers(r)
        })
        .sum()
}

pub fn b() -> usize {
    let re = Regex::new(r"mul\(\d{1,3},\d{1,3}\)|do\(\)|don't\(\)").unwrap();
    let text = misc::text();

    let mut enabled = true;
    re.find_iter(&text)
        .map(|result| {
            let result = result.as_str();

            if result == "don't()" {
                enabled = false;
            } else if result == "do()" {
                enabled = true;
            } else if enabled {
                let (l, r) = result.split_once(",").unwrap();
                return strip_numbers(l) * strip_numbers(r);
            }

            0
        })
        .sum()
}
