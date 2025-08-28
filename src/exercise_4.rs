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

pub fn a() -> usize {
    let word = "XMAS";
    let board: Vec<Vec<char>> = misc::text()
        .trim()
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

    count
}

pub fn b() -> usize {
    0
}
