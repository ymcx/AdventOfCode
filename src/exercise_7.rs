use crate::misc;

pub fn a() -> usize {
    misc::lines()
        .map(|line| {
            let line = line.unwrap();
            let (test, values) = line.split_once(": ").unwrap();
            let test: usize = test.parse().unwrap();
            let values: Vec<usize> = values.split(" ").map(|i| i.parse().unwrap()).collect();

            let n = values.len() - 1;
            let mut results = Vec::new();
            for i in 0..usize::pow(2, n as u32) {
                let mask: Vec<usize> = (0..n).map(|bit| (i >> bit) & 1).collect();
                let mut result = values[0];
                mask.iter().enumerate().for_each(|(i, value)| match value {
                    0 => result *= values[i + 1],
                    1 => result += values[i + 1],
                    _ => {}
                });
                results.push(result);
            }

            if results.contains(&test) {
                return test;
            }

            0
        })
        .sum()
}

pub fn b() -> usize {
    0
}
