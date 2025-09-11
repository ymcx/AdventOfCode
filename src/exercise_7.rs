use crate::misc;

fn create_mask(base: usize, seed: usize, length: usize) -> Vec<usize> {
    let mut value = seed;
    (0..length)
        .map(|_| {
            let bit = value % base;
            value /= base;
            bit
        })
        .collect()
}

fn parse_values(path: &str) -> Vec<(usize, Vec<usize>)> {
    misc::lines(path)
        .map(|line| {
            let line = line.unwrap();
            let (test, values) = line.split_once(": ").unwrap();
            let test: usize = test.parse().unwrap();
            let values: Vec<usize> = values.split(" ").map(|i| i.parse().unwrap()).collect();
            (test, values)
        })
        .collect()
}

fn get_result(path: &str, base: usize) -> usize {
    parse_values(path)
        .iter()
        .map(|(test, values)| {
            let operator_places = values.len() - 1;
            let combinations = usize::pow(base, operator_places as u32);
            let mut results = Vec::new();
            for i in 0..combinations {
                let mask = create_mask(base, i, operator_places);
                let mut result = values[0];
                mask.iter().enumerate().for_each(|(i, value)| match value {
                    0 => result *= values[i + 1],
                    1 => result += values[i + 1],
                    2 => {
                        let length = values[i + 1].to_string().len();
                        let shifted_result = result * usize::pow(10, length as u32);
                        result = shifted_result + values[i + 1];
                    }
                    _ => {}
                });
                results.push(result);
            }

            if results.contains(&test) {
                return test;
            }

            &0
        })
        .sum()
}

pub fn a(path: &str) -> String {
    get_result(path, 2).to_string()
}

pub fn b(path: &str) -> String {
    get_result(path, 3).to_string()
}

#[test]
fn test() {
    assert!(a("input/exercise_7.txt") == "7710205485870");
    assert!(b("input/exercise_7.txt") == "20928985450275");
}
