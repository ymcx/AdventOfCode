use crate::misc;
use std::collections::HashSet;

fn adjacent(
    board: &Vec<Vec<char>>,
    travelled: &mut HashSet<(i32, i32)>,
    point: (i32, i32),
    char: char,
) -> usize {
    if travelled.contains(&point) {
        return 0;
    }

    let (y, x) = point;
    let (y_max, x_max) = (board.len() as i32, board[0].len() as i32);
    if y < 0 || x < 0 || y >= y_max || x >= x_max || board[y as usize][x as usize] != char {
        return 1;
    }

    travelled.insert(point);
    vec![(y + 1, x), (y - 1, x), (y, x + 1), (y, x - 1)]
        .into_iter()
        .map(|point| adjacent(board, travelled, point, char))
        .sum()
}

pub fn a() -> usize {
    let board: Vec<Vec<char>> = misc::lines()
        .map(|line| line.unwrap().chars().collect())
        .collect();
    let mut total_travelled: HashSet<(i32, i32)> = HashSet::new();

    board
        .iter()
        .enumerate()
        .map(|(y, row)| {
            row.iter()
                .enumerate()
                .map(|(x, &char)| {
                    let point = (y as i32, x as i32);
                    if total_travelled.contains(&point) {
                        return 0;
                    }

                    let mut travelled = HashSet::new();
                    let perimeter = adjacent(&board, &mut travelled, point, char);
                    let area = travelled.len();
                    total_travelled.extend(travelled);

                    perimeter * area
                })
                .sum::<usize>()
        })
        .sum()
}

pub fn b() -> usize {
    0
}
