use crate::misc;
use std::collections::HashSet;

enum Direction {
    Up,
    Down,
    Left,
    Right,
}

impl Direction {
    pub fn get(&self) -> (i32, i32) {
        match self {
            Self::Up => (-1, 0),
            Self::Down => (1, 0),
            Self::Left => (0, -1),
            Self::Right => (0, 1),
        }
    }

    pub fn turn(&self) -> Self {
        match self {
            Self::Up => Self::Right,
            Self::Down => Self::Left,
            Self::Left => Self::Up,
            Self::Right => Self::Down,
        }
    }
}

fn parse_board() -> (Vec<Vec<char>>, (i32, i32)) {
    let board: Vec<Vec<_>> = misc::lines()
        .map(|line| line.unwrap().chars().collect())
        .collect();
    let dimensions = (board.len() as i32, board[0].len() as i32);

    (board, dimensions)
}

fn parse_obstacles(board: &Vec<Vec<char>>) -> (Vec<(i32, i32)>, (i32, i32)) {
    let mut start_position = (0, 0);
    let mut obstacles = Vec::new();
    for (y, row) in board.iter().enumerate() {
        for (x, char) in row.iter().enumerate() {
            let position = (y as i32, x as i32);
            match char {
                '^' => start_position = position,
                '#' => obstacles.push(position),
                _ => continue,
            }
        }
    }

    (obstacles, start_position)
}

fn unique_location_amount(history: &HashSet<((i32, i32), (i32, i32))>) -> usize {
    history.iter().map(|(i, _)| i).collect::<HashSet<_>>().len()
}

fn travel(
    start_position: (i32, i32),
    dimensions: (i32, i32),
    obstacles: &Vec<(i32, i32)>,
) -> usize {
    let mut position = start_position;
    let mut direction = Direction::Up;
    let mut history = HashSet::new();
    let (max_y, max_x) = dimensions;

    loop {
        let position_with_direction = (position, direction.get());
        if history.contains(&position_with_direction) {
            return 0;
        }
        history.insert(position_with_direction);

        let new_position = (
            position.0 + direction.get().0,
            position.1 + direction.get().1,
        );
        let (y, x) = new_position;
        if y < 0 || x < 0 || max_y <= y || max_x <= x {
            return unique_location_amount(&history);
        }

        if obstacles.contains(&new_position) {
            direction = direction.turn();
            continue;
        }

        position = new_position;
    }
}

pub fn a() -> usize {
    let (board, dimensions) = parse_board();
    let (obstacles, start_position) = parse_obstacles(&board);

    travel(start_position, dimensions, &obstacles)
}

pub fn b() -> usize {
    let (board, dimensions) = parse_board();
    let (obstacles, start_position) = parse_obstacles(&board);

    board
        .iter()
        .enumerate()
        .map(|(y, row)| {
            row.iter()
                .enumerate()
                .filter(|(x, char)| {
                    if "^#".contains(**char) {
                        return false;
                    }

                    let new_obstacle = (y as i32, *x as i32);
                    let new_obstacles = obstacles
                        .iter()
                        .cloned()
                        .chain(std::iter::once(new_obstacle))
                        .collect();

                    travel(start_position, dimensions, &new_obstacles) == 0
                })
                .count()
        })
        .sum()
}
