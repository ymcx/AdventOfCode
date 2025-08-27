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
    0
}
