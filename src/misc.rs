use std::{
    env,
    fs::File,
    io::{BufRead, BufReader, Lines},
};

pub fn args() -> Vec<String> {
    env::args().collect()
}

pub fn lines() -> Lines<BufReader<File>> {
    let path = args()[2].clone();
    let file = File::open(path).unwrap();
    let reader = BufReader::new(file);
    reader.lines()
}
