use crate::misc;
use std::collections::{HashMap, HashSet};

fn get_secret(mut secret: usize) -> usize {
    secret = (secret * 64 ^ secret) % 16777216;
    secret = (secret / 32 ^ secret) % 16777216;
    secret = (secret * 2048 ^ secret) % 16777216;
    secret
}

fn get_secret_n(mut secret: usize, n: usize) -> usize {
    for _ in 0..n {
        secret = get_secret(secret)
    }
    secret
}

pub fn a(path: &str) -> String {
    misc::lines(path)
        .iter()
        .map(|line| get_secret_n(line.parse().unwrap(), 2000))
        .sum::<usize>()
        .to_string()
}

pub fn b(path: &str) -> String {
    let mut sequences = HashMap::new();
    for line in misc::lines(path) {
        let mut secret = line.parse().unwrap();
        let mut secret_char = secret % 10;
        let mut seen = HashSet::new();
        let mut sequence = [0; 4];

        for i in 0..2000 {
            secret = get_secret(secret);
            sequence.rotate_left(1);
            sequence[3] = (secret % 10) as i32 - secret_char as i32;
            secret_char = secret % 10;

            if seen.contains(&sequence) || i < 3 {
                continue;
            }

            seen.insert(sequence);
            *sequences.entry(sequence).or_insert(0) += secret_char;
        }
    }

    sequences.values().max().unwrap().to_string()
}
