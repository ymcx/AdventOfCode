use crate::misc::{self, SignedPoint};

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

fn get_coordinates_numeric(char: char) -> SignedPoint {
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
fn get_coordinates_directional(char: char) -> SignedPoint {
    match char {
        '^' => (1, 0),
        'A' => (2, 0),
        '<' => (0, 1),
        'v' => (1, 1),
        '>' => (2, 1),
        _ => panic!(),
    }
}

fn keypad(char: char, (x, y): &mut SignedPoint, numeric: bool) -> Vec<char> {
    let mut inputs = Vec::new();
    let (next_x, next_y) = if numeric {
        get_coordinates_numeric(char)
    } else {
        get_coordinates_directional(char)
    };
    let delta = (next_x - *x, next_y - *y);

    // https://www.reddit.com/r/adventofcode/comments/1hjgyps/2024_day_21_part_2_i_got_greedyish/
    if (numeric && *x == 0 && next_y == 3) || (!numeric && *x == 0 && next_y == 0) {
        add_inputs(delta, &mut inputs, false);
    } else if (numeric && *y == 3 && next_x == 0) || (!numeric && *y == 0 && next_x == 0) {
        add_inputs(delta, &mut inputs, true);
    } else if delta.0 < 0 {
        add_inputs(delta, &mut inputs, false);
    } else {
        add_inputs(delta, &mut inputs, true);
    }

    (*x, *y) = (next_x, next_y);

    inputs
}

fn line_to_complexity(line: &str) -> usize {
    let mut robots = ((2, 3), (2, 0), (2, 0));
    let movements: String = line
        .chars()
        .map(|char| keypad(char, &mut robots.0, true))
        .flatten()
        .map(|char| keypad(char, &mut robots.1, false))
        .flatten()
        .map(|char| keypad(char, &mut robots.2, false))
        .flatten()
        .collect();
    let len = movements.len();
    let num: usize = line
        .chars()
        .filter(|char| char.is_digit(10))
        .collect::<String>()
        .parse()
        .unwrap();

    len * num
}

pub fn a(path: &str) -> String {
    misc::lines(path)
        .iter()
        .map(|line| line_to_complexity(line))
        .sum::<usize>()
        .to_string()
}

pub fn b(_path: &str) -> String {
    "".to_string()
}

#[test]
fn test() {
    assert!(a("input/exercise_21.txt") == "174124");
    assert!(b("input/exercise_21.txt") != "");
}
