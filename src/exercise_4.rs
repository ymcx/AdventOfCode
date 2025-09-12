use crate::misc;

fn found_word(
    direction: isize,
    origin: (isize, isize),
    word: &str,
    board: &Vec<Vec<char>>,
) -> bool {
    let (y_direction, x_direction) = (direction / 3 - 1, direction % 3 - 1);
    let (y_max, x_max) = (board.len() as isize, board[0].len() as isize);
    let (y_origin, x_origin) = origin;

    let found: String = (1..word.len())
        .map(|n| {
            let n = n as isize;
            let (y, x) = (n * y_direction + y_origin, n * x_direction + x_origin);
            if y < 0 || x < 0 || y_max <= y || x_max <= x {
                return ' ';
            }

            board[y as usize][x as usize]
        })
        .collect();

    found == word[1..]
}

fn is_match(left: char, right: char) -> bool {
    (left == 'M' && right == 'S') || (left == 'S' && right == 'M')
}

fn found_pair(origin: (isize, isize), board: &Vec<Vec<char>>) -> bool {
    let (y_max, x_max) = (board.len() as isize, board[0].len() as isize);
    let (y_origin, x_origin) = origin;
    let (y1, x1) = (y_origin + 1, x_origin + 1);
    let (y2, x2) = (y_origin - 1, x_origin + 1);
    let (y3, x3) = (y_origin - 1, x_origin - 1);
    let (y4, x4) = (y_origin + 1, x_origin - 1);

    if y3 < 0 || x3 < 0 || y_max <= y1 || x_max <= x1 {
        return false;
    }

    let top_right = board[y1 as usize][x1 as usize];
    let bottom_right = board[y2 as usize][x2 as usize];
    let bottom_left = board[y3 as usize][x3 as usize];
    let top_left = board[y4 as usize][x4 as usize];

    is_match(top_right, bottom_left) && is_match(bottom_right, top_left)
}

pub fn a(path: &str) -> String {
    let word = "XMAS";
    let board: Vec<Vec<char>> = misc::text(path)
        .split("\n")
        .map(|row| row.chars().collect())
        .collect();

    let mut count = 0;
    for (y, row) in board.iter().enumerate() {
        for (x, char) in row.iter().enumerate() {
            if *char != 'X' {
                continue;
            }

            let origin = (y as isize, x as isize);
            for direction in 0..9 {
                if direction == 4 {
                    continue;
                }

                if found_word(direction, origin, word, &board) {
                    count += 1;
                }
            }
        }
    }

    count.to_string()
}

pub fn b(path: &str) -> String {
    let board: Vec<Vec<char>> = misc::text(path)
        .split("\n")
        .map(|row| row.chars().collect())
        .collect();

    let mut count = 0;
    for (y, row) in board.iter().enumerate() {
        for (x, char) in row.iter().enumerate() {
            if *char != 'A' {
                continue;
            }

            let origin = (y as isize, x as isize);
            if found_pair(origin, &board) {
                count += 1;
            }
        }
    }

    count.to_string()
}

#[test]
fn test() {
    assert!(a("input/exercise_4.txt") == "2554");
    assert!(b("input/exercise_4.txt") == "1916");
}
