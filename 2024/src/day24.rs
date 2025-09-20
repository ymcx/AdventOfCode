use crate::misc;
use itertools::Itertools;
use std::collections::{HashMap, HashSet};

fn parse(
    path: &str,
) -> (
    HashMap<String, usize>,
    Vec<(String, String, String, String)>,
) {
    let text = misc::text(path);
    let (registers, arguments) = text.split_once("\n\n").unwrap();
    let registers = registers
        .lines()
        .map(|line| {
            let (register, value) = line.split_once(": ").unwrap();
            (register.to_string(), value.parse().unwrap())
        })
        .collect();
    let arguments = arguments
        .lines()
        .map(|line| {
            let (arg1, op, arg2, _, out) = line.split(" ").map_into().collect_tuple().unwrap();
            (arg1, op, arg2, out)
        })
        .collect();

    (registers, arguments)
}

pub fn a(path: &str) -> String {
    let (mut registers, mut arguments) = parse(path);

    while !arguments.is_empty() {
        arguments.retain(|(arg1, op, arg2, out)| {
            if let (Some(arg1), Some(arg2)) = (registers.get(arg1), registers.get(arg2)) {
                let result = match op.as_str() {
                    "XOR" => arg1 ^ arg2,
                    "OR" => arg1 | arg2,
                    "AND" => arg1 & arg2,
                    _ => panic!(),
                };
                registers.insert(out.to_string(), result);
                return false;
            }

            true
        });
    }

    registers
        .iter()
        .filter(|(register, _)| register.starts_with("z"))
        .sorted()
        .map(|(_, value)| value)
        .enumerate()
        .fold(0, |acc, (i, bit)| acc + (bit << i))
        .to_string()
}

fn first_char(input: &str) -> String {
    input.chars().next().unwrap().to_string()
}

// https://www.reddit.com/r/adventofcode/comments/1hl698z/comment/m3kt1je
pub fn b(path: &str) -> String {
    let (_, arguments) = parse(path);
    let mut swapped = HashSet::new();

    for (arg1, op, arg2, out) in arguments.iter() {
        let (arg1_f, arg2_f, out_f) = (first_char(arg1), first_char(arg2), first_char(out));
        let xyz = ["x", "y", "z"];

        if (out_f == "z" && out != "z45" && op != "XOR")
            || (op == "XOR"
                && !xyz.contains(&arg1_f.as_str())
                && !xyz.contains(&arg2_f.as_str())
                && !xyz.contains(&out_f.as_str()))
        {
            swapped.insert(out);
            continue;
        }

        for (arg1_2, op_2, arg2_2, _) in arguments.iter() {
            if (out == arg1_2 || out == arg2_2)
                && ((op == "AND" && op_2 != "OR" && arg1 != "x00" && arg2 != "x00")
                    || (op == "XOR" && op_2 == "OR"))
            {
                swapped.insert(out);
            }
        }
    }

    swapped.iter().sorted().join(",")
}
