use std::{
    env,
    fs::File,
    io::{BufRead, BufReader, Lines, Read},
};

fn get_reader() -> BufReader<File> {
    let path = args()[2].clone();
    let file = File::open(path).unwrap();
    BufReader::new(file)
}

pub fn args() -> Vec<String> {
    env::args().collect()
}

pub fn lines() -> Lines<BufReader<File>> {
    get_reader().lines()
}

pub fn text() -> String {
    let mut text = String::new();
    get_reader().read_to_string(&mut text).unwrap();
    text
}
