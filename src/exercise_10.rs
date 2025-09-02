use crate::misc;
use std::collections::HashSet;

fn parse_board() -> (Vec<Vec<i32>>, (i32, i32)) {
    let board: Vec<Vec<i32>> = misc::lines()
        .map(|line| {
            line.unwrap()
                .chars()
                .map(|char| char.to_digit(10).unwrap() as i32)
                .collect()
        })
        .collect();
    let dimensions = (board.len() as i32, board[0].len() as i32);

    (board, dimensions)
}

fn find_starting_points(board: &Vec<Vec<i32>>) -> Vec<(i32, i32)> {
    let mut starting_points = Vec::new();
    board.iter().enumerate().for_each(|(y, line)| {
        line.iter().enumerate().for_each(|(x, char)| {
            if *char != 0 {
                return;
            }

            let starting_point = (y as i32, x as i32);
            starting_points.push(starting_point);
        });
    });

    starting_points
}

fn is_legal_move(
    board: &Vec<Vec<i32>>,
    dimensions: (i32, i32),
    point: (i32, i32),
    next_point: (i32, i32),
) -> bool {
    let (max_y, max_x) = dimensions;
    let (y, x) = point;
    let (next_y, next_x) = next_point;

    if next_y < 0 || next_x < 0 || next_y >= max_y || next_x >= max_x {
        return false;
    }

    let value = board[y as usize][x as usize];
    let next_value = board[next_y as usize][next_x as usize];
    next_value == value + 1
}

fn find_paths(
    board: &Vec<Vec<i32>>,
    dimensions: (i32, i32),
    point: (i32, i32),
) -> HashSet<(i32, i32)> {
    let (y, x) = point;
    let value = board[y as usize][x as usize];
    if value == 9 {
        return [point].into_iter().collect();
    }

    let next_points = vec![(y + 1, x), (y - 1, x), (y, x + 1), (y, x - 1)];
    next_points
        .into_iter()
        .filter(|&next_point| is_legal_move(board, dimensions, point, next_point))
        .map(|next_point| find_paths(board, dimensions, next_point))
        .flatten()
        .collect()
}

pub fn a() -> usize {
    let (board, dimensions) = parse_board();
    let starting_points = find_starting_points(&board);
    starting_points
        .into_iter()
        .map(|starting_point| find_paths(&board, dimensions, starting_point).len())
        .sum()
}

pub fn b() -> usize {
    0
}
