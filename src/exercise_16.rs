use crate::misc;
use std::{
    cmp,
    collections::{HashMap, HashSet},
};

fn parse_board() -> (HashSet<(usize, usize)>, (usize, usize), (usize, usize)) {
    let mut board = HashSet::new();
    let mut start = (0, 0);
    let mut end = (0, 0);
    for (y, row) in misc::lines().enumerate() {
        for (x, char) in row.unwrap().chars().enumerate() {
            let point = (y, x);
            match char {
                '#' => {
                    board.insert(point);
                }
                'S' => {
                    start = point;
                }
                'E' => {
                    end = point;
                }
                _ => {}
            }
        }
    }

    (board, start, end)
}

fn next_points(
    board: &HashSet<(usize, usize)>,
    point: (usize, usize),
    steps: usize,
    travelled: &HashMap<(usize, usize), usize>,
) -> Vec<(usize, (usize, usize))> {
    [(0, 1), (-1, 0), (0, -1), (1, 0)]
        .into_iter()
        .enumerate()
        .filter_map(|(direction, delta)| {
            let next_point = (
                point.0.saturating_add_signed(delta.0),
                point.1.saturating_add_signed(delta.1),
            );

            if board.contains(&next_point) {
                return None;
            }

            if travelled.get(&next_point).map_or(true, |&s| steps < s) {
                return Some((direction, next_point));
            }

            None
        })
        .collect()
}

fn get_min_steps(
    board: &HashSet<(usize, usize)>,
    start: (usize, usize),
    end: (usize, usize),
) -> usize {
    let mut players = vec![(start, 0, 0)];
    let mut travelled = HashMap::new();
    let mut min_steps = usize::MAX;

    while let Some(mut player) = players.pop() {
        loop {
            let (point, direction, steps) = player;
            travelled.insert(point, steps);

            if point == end {
                min_steps = cmp::min(steps, min_steps);
                break;
            }

            let next_points = next_points(board, point, steps, &travelled);
            if next_points.len() == 0 {
                break;
            }

            for (i, (next_direction, next_point)) in next_points.into_iter().enumerate() {
                let next_steps = steps + if direction == next_direction { 1 } else { 1001 };
                let next_player = (next_point, next_direction, next_steps);

                if i == 0 {
                    player = next_player;
                    continue;
                }

                players.push(next_player);
            }
        }
    }

    min_steps
}

pub fn a() -> usize {
    let (board, start, end) = parse_board();
    get_min_steps(&board, start, end)
}

pub fn b() -> usize {
    0
}
