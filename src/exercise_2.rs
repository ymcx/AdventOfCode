use std::io::Error;

use crate::misc;

fn is_sorted(values: &Vec<usize>) -> bool {
    values.is_sorted_by(|a, b| a <= b) || values.is_sorted_by(|a, b| a >= b)
}

fn is_gradual(values: &Vec<usize>) -> bool {
    values.is_sorted_by(|a, b| {
        let diff = a.abs_diff(*b);
        1 <= diff && diff <= 3
    })
}

fn collect_levels(line: &Result<String, Error>) -> Vec<usize> {
    line.as_ref()
        .unwrap()
        .split_whitespace()
        .map(|i| i.parse().unwrap())
        .collect()
}

pub fn a() -> usize {
    misc::lines()
        .filter(|line| {
            let values = &collect_levels(line);
            is_sorted(values) && is_gradual(values)
        })
        .count()
}

pub fn b() -> usize {
    misc::lines()
        .filter(|line| {
            let values = &collect_levels(line);
            (0..values.len())
                .map(|delete_i| {
                    values
                        .into_iter()
                        .enumerate()
                        .filter(|(i, _)| *i != delete_i)
                        .map(|(_, a)| *a)
                        .collect()
                })
                .any(|v| is_sorted(&v) && is_gradual(&v))
        })
        .count()
}
