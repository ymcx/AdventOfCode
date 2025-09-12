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

fn collect_levels(line: &str) -> Vec<usize> {
    line.split_whitespace()
        .map(|i| i.parse().unwrap())
        .collect()
}

pub fn a(path: &str) -> String {
    misc::lines(path)
        .iter()
        .filter(|line| {
            let values = &collect_levels(line);
            is_sorted(values) && is_gradual(values)
        })
        .count()
        .to_string()
}

pub fn b(path: &str) -> String {
    misc::lines(path)
        .iter()
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
        .to_string()
}

#[test]
fn test() {
    assert!(a("input/exercise_2.txt") == "549");
    assert!(b("input/exercise_2.txt") == "589");
}
