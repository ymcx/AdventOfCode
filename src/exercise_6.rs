use crate::misc::{self, SignedPoint};
use std::collections::HashSet;

enum Direction {
    Up,
    Down,
    Left,
    Right,
}

impl Direction {
    pub fn get(&self) -> SignedPoint {
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

fn parse_board(path: &str) -> (Vec<Vec<char>>, SignedPoint) {
    let board: Vec<Vec<_>> = misc::lines(path)
        .iter()
        .map(|line| line.chars().collect())
        .collect();
    let dimensions = (board.len() as i32, board[0].len() as i32);

    (board, dimensions)
}

fn parse_obstacles(board: &Vec<Vec<char>>) -> (Vec<SignedPoint>, SignedPoint) {
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

fn unique_locations(history: &HashSet<(SignedPoint, SignedPoint)>) -> HashSet<SignedPoint> {
    history.iter().map(|(i, _)| *i).collect()
}

fn travel(
    start_position: SignedPoint,
    dimensions: SignedPoint,
    obstacles: &Vec<SignedPoint>,
) -> HashSet<SignedPoint> {
    let mut position = start_position;
    let mut direction = Direction::Up;
    let mut history = HashSet::new();
    let (max_y, max_x) = dimensions;

    loop {
        let position_with_direction = (position, direction.get());
        if history.contains(&position_with_direction) {
            return HashSet::new();
        }
        history.insert(position_with_direction);

        let new_position = (
            position.0 + direction.get().0,
            position.1 + direction.get().1,
        );
        let (y, x) = new_position;
        if y < 0 || x < 0 || max_y <= y || max_x <= x {
            return unique_locations(&history);
        }

        if obstacles.contains(&new_position) {
            direction = direction.turn();
            continue;
        }

        position = new_position;
    }
}

pub fn a(path: &str) -> String {
    let (board, dimensions) = parse_board(path);
    let (obstacles, start_position) = parse_obstacles(&board);

    travel(start_position, dimensions, &obstacles)
        .len()
        .to_string()
}

pub fn b(path: &str) -> String {
    let (board, dimensions) = parse_board(path);
    let (obstacles, start_position) = parse_obstacles(&board);
    let route = travel(start_position, dimensions, &obstacles);

    board
        .iter()
        .enumerate()
        .map(|(y, row)| {
            row.iter()
                .enumerate()
                .filter(|(x, _)| {
                    let new_obstacle = (y as i32, *x as i32);
                    if !route.contains(&new_obstacle) {
                        return false;
                    }

                    let new_obstacles = obstacles
                        .iter()
                        .cloned()
                        .chain(std::iter::once(new_obstacle))
                        .collect();

                    travel(start_position, dimensions, &new_obstacles).len() == 0
                })
                .count()
        })
        .sum::<usize>()
        .to_string()
}

#[test]
fn test() {
    assert!(a("input/exercise_6.txt") == "5067");
    assert!(b("input/exercise_6.txt") == "1793");
}
