use crate::misc::{self, SignedPoint};
use std::{collections::HashMap, str::Chars};

const PATH: [[&str; 5]; 5] = [
    ["A", ">^A", ">A", ">>^A", ">>A"],
    ["v<A", "A", "vA", ">A", "v>A"],
    ["<A", "^A", "A", "^>A", ">A"],
    ["v<<A", "<A", "<vA", "A", "vA"],
    ["<<A", "<^A", "<A", "^A", "A"],
];

const fn get_index_directional(char: u8) -> usize {
    match char {
        b'<' => 0,
        b'^' => 1,
        b'v' => 2,
        b'A' => 3,
        b'>' => 4,
        _ => panic!(),
    }
}

const fn get_coordinates_directional(char: u8) -> SignedPoint {
    match char {
        b'^' => (1, 0),
        b'A' => (2, 0),
        b'<' => (0, 1),
        b'v' => (1, 1),
        b'>' => (2, 1),
        _ => panic!(),
    }
}

const fn get_coordinates_numeric(char: u8) -> SignedPoint {
    match char {
        b'7' => (0, 0),
        b'8' => (1, 0),
        b'9' => (2, 0),
        b'4' => (0, 1),
        b'5' => (1, 1),
        b'6' => (2, 1),
        b'1' => (0, 2),
        b'2' => (1, 2),
        b'3' => (2, 2),
        b'0' => (1, 3),
        b'A' => (2, 3),
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

fn numeric_keypad(char: u8, (x, y): &mut SignedPoint) -> Vec<char> {
    let mut inputs = Vec::new();
    let (next_x, next_y) = get_coordinates_numeric(char);
    let delta = (next_x - *x, next_y - *y);

    // https://www.reddit.com/r/adventofcode/comments/1hjgyps/2024_day_21_part_2_i_got_greedyish/
    let vertical_first = !(*x == 0 && next_y == 3 || delta.0 < 0) || (*y == 3 && next_x == 0);
    add_inputs(delta, &mut inputs, vertical_first);

    (*x, *y) = (next_x, next_y);

    inputs
}

fn directional_keypad(
    char: u8,
    (x, y): &mut SignedPoint,
    old_index: &mut usize,
    index: &mut usize,
) -> Chars<'static> {
    *old_index = *index;
    *index = get_index_directional(char);

    let (next_x, next_y) = get_coordinates_directional(char);
    (*x, *y) = (next_x, next_y);

    PATH[*old_index][*index].chars()
}

fn line_to_complexity(line: &str, n: usize, cache: &mut HashMap<String, Vec<char>>) -> usize {
    let mut robots = ((2, 3), (2, 0));
    let mut movements: String = line
        .chars()
        .map(|c| numeric_keypad(c as u8, &mut robots.0))
        .flatten()
        .collect();

    let mut index = 3;
    let mut old_index = index;
    for _ in 0..n {
        movements = movements
            .split_inclusive('A')
            .map(|part| {
                if let Some(cached) = cache.get(part) {
                    return cached.clone();
                }

                let result: Vec<_> = part
                    .chars()
                    .map(|c| directional_keypad(c as u8, &mut robots.1, &mut old_index, &mut index))
                    .flatten()
                    .collect();

                cache.insert(part.to_string(), result.clone());
                result
            })
            .flatten()
            .collect()
    }

    let len = movements.len();
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
    solve(path, 20)
}

#[test]
fn test() {
    assert!(a("input/exercise_21.txt") == "174124");
    // assert!(b("input/exercise_21.txt") != "");
}
