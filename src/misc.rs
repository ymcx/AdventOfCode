use std::{
    env,
    fs::File,
    io::{BufRead, BufReader, Read},
};

pub type Point = (usize, usize);
pub type SignedPoint = (i32, i32);

fn get_reader(path: &str) -> BufReader<File> {
    let file = File::open(path).unwrap();
    BufReader::new(file)
}

pub fn args() -> Vec<String> {
    env::args().collect()
}

pub fn nth_line(path: &str, line: usize) -> String {
    lines(path)[line].to_string()
}

pub fn count_lines(path: &str) -> usize {
    lines(path).len()
}

pub fn lines(path: &str) -> Vec<String> {
    get_reader(path).lines().map(|line| line.unwrap()).collect()
}

pub fn text(path: &str) -> String {
    let mut text = String::new();
    get_reader(path).read_to_string(&mut text).unwrap();
    text.trim().to_string()
}
