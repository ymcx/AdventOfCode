use crate::misc;

fn get_secret(line: &str, n: usize) -> usize {
    let mut secret = line.parse().unwrap();
    let modulo = 16777216;
    for _ in 0..n {
        secret = ((secret * 64) ^ secret) % modulo;
        secret = ((secret / 32) ^ secret) % modulo;
        secret = ((secret * 2048) ^ secret) % modulo;
    }

    secret
}

pub fn a(path: &str) -> String {
    misc::lines(path)
        .iter()
        .map(|line| get_secret(line, 2000))
        .sum::<usize>()
        .to_string()
}

pub fn b(_path: &str) -> String {
    "".to_string()
}

#[test]
fn test() {
    assert!(a("input/exercise_22.txt") == "20071921341");
    // assert!(b("input/exercise_22.txt") != 0);
}
