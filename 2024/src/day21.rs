use crate::misc::{self, SignedPoint};
use std::collections::HashMap;

const PATH: [[&str; 5]; 5] = [
    ["A", ">^A", ">A", ">>^A", ">>A"],
    ["v<A", "A", "vA", ">A", "v>A"],
    ["<A", "^A", "A", "^>A", ">A"],
    ["v<<A", "<A", "<vA", "A", "vA"],
    ["<<A", "<^A", "<A", "^A", "A"],
];

const fn get_index_directional(char: char) -> usize {
    match char {
        '<' => 0,
        '^' => 1,
        'v' => 2,
        'A' => 3,
        '>' => 4,
        _ => panic!(),
    }
}

const fn get_coordinates_directional(char: char) -> SignedPoint {
    match char {
        '^' => (1, 0),
        'A' => (2, 0),
        '<' => (0, 1),
        'v' => (1, 1),
        '>' => (2, 1),
        _ => panic!(),
    }
}

const fn get_coordinates_numeric(char: char) -> SignedPoint {
    match char {
        '7' => (0, 0),
        '8' => (1, 0),
        '9' => (2, 0),
        '4' => (0, 1),
        '5' => (1, 1),
        '6' => (2, 1),
        '1' => (0, 2),
        '2' => (1, 2),
        '3' => (2, 2),
        '0' => (1, 3),
        'A' => (2, 3),
        _ => panic!(),
    }
}

fn add_inputs((dx, dy): SignedPoint, inputs: &mut Vec<char>, vertical_first: bool) {
    let letters_horizontal = vec![if dx.is_negative() { '<' } else { '>' }; dx.abs() as usize];
    let letters_vertical = vec![if dy.is_negative() { '^' } else { 'v' }; dy.abs() as usize];

    if vertical_first {
        inputs.extend(letters_vertical);
        inputs.extend(letters_horizontal);
    } else {
        inputs.extend(letters_horizontal);
        inputs.extend(letters_vertical);
    }
    inputs.push('A');
}

fn numeric_keypad(char: char, (x, y): &mut SignedPoint) -> Vec<char> {
    let mut inputs = Vec::new();
    let (next_x, next_y) = get_coordinates_numeric(char);
    let delta = (next_x - *x, next_y - *y);

    // https://www.reddit.com/r/adventofcode/comments/1hjgyps/2024_day_21_part_2_i_got_greedyish/
    let vertical_first = !(*x == 0 && next_y == 3 || delta.0 < 0) || (*y == 3 && next_x == 0);
    add_inputs(delta, &mut inputs, vertical_first);

    (*x, *y) = (next_x, next_y);

    inputs
}

fn split(movements: &str, n: usize, cache: &mut HashMap<(String, usize), usize>) -> usize {
    if n == 0 {
        return movements.len();
    }

    let cache_key = (movements.to_string(), n);
    if let Some(&cached) = cache.get(&cache_key) {
        return cached;
    }

    let mut old = 3;
    let mut cur = 3;
    let mut robot = (2, 0);
    let sum = movements
        .chars()
        .map(|c| {
            old = cur;
            cur = get_index_directional(c);
            robot = get_coordinates_directional(c);
            split(PATH[old][cur], n - 1, cache)
        })
        .sum();

    cache.insert(cache_key, sum);
    sum
}

fn line_to_complexity(line: &str, n: usize, cache: &mut HashMap<(String, usize), usize>) -> usize {
    let mut robot = (2, 3);
    let movements: String = line
        .chars()
        .flat_map(|c| numeric_keypad(c, &mut robot))
        .collect();

    let len = split(&movements, n, cache);
    let num: usize = line
        .chars()
        .filter(|char| char.is_digit(10))
        .collect::<String>()
        .parse()
        .unwrap();

    len * num
}

fn solve(path: &str, n: usize) -> String {
    let mut cache = HashMap::new();
    misc::lines(path)
        .iter()
        .map(|line| line_to_complexity(line, n, &mut cache))
        .sum::<usize>()
        .to_string()
}

pub fn a(path: &str) -> String {
    solve(path, 2)
}

pub fn b(path: &str) -> String {
    solve(path, 25)
}
