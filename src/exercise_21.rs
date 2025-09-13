use crate::misc::{self, SignedPoint};

fn add_inputs(delta: SignedPoint, inputs: &mut Vec<char>, vertical: bool) {
    let (delta_n, letters) = if vertical {
        (delta.1, ('^', 'v'))
    } else {
        (delta.0, ('<', '>'))
    };
    for _ in 0..delta_n.abs() {
        if delta_n < 0 {
            inputs.push(letters.0);
        } else {
            inputs.push(letters.1);
        }
    }
}

fn get_coordinates_numeric(char: char) -> (SignedPoint, SignedPoint) {
    let death_point = (0, 3);
    let point = match char {
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
    };
    (point, death_point)
}
fn get_coordinates_directional(char: char) -> (SignedPoint, SignedPoint) {
    let death_point = (0, 0);
    let point = match char {
        '^' => (1, 0),
        'A' => (2, 0),
        '<' => (0, 1),
        'v' => (1, 1),
        '>' => (2, 1),
        _ => panic!(),
    };
    (point, death_point)
}

fn keypad(char: char, current_point: &mut SignedPoint, numeric: bool) -> Vec<char> {
    let (char_point, death_point) = if numeric {
        get_coordinates_numeric(char)
    } else {
        get_coordinates_directional(char)
    };

    let delta = (
        char_point.0 - current_point.0,
        char_point.1 - current_point.1,
    );

    let mut inputs = Vec::new();
    if (char_point.0, current_point.1) == death_point {
        add_inputs(delta, &mut inputs, true);
        add_inputs(delta, &mut inputs, false);
    } else {
        add_inputs(delta, &mut inputs, false);
        add_inputs(delta, &mut inputs, true);
    }
    inputs.push('A');
    *current_point = char_point;

    inputs
}

pub fn a(path: &str) -> String {
    let mut robots = ((2, 3), (2, 0), (2, 0));
    misc::lines(path)
        .iter()
        .map(|line| {
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
        })
        .sum::<usize>()
        .to_string()
}

pub fn b(_path: &str) -> String {
    "".to_string()
}

// #[test]
// fn test() {
//     assert!(a("input/exercise_21.txt") != 0);
//     assert!(b("input/exercise_21.txt") != 0);
// }

// 179008 wrong
// 180372 high
