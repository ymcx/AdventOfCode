use std::{
    env,
    fs::File,
    io::{BufRead, BufReader, Lines, Read},
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
    lines(path).nth(line).unwrap().unwrap()
}

pub fn count_lines(path: &str) -> usize {
    lines(path).count()
}

pub fn lines(path: &str) -> Lines<BufReader<File>> {
    get_reader(path).lines()
}

pub fn text(path: &str) -> String {
    let mut text = String::new();
    get_reader(path).read_to_string(&mut text).unwrap();
    text
}
