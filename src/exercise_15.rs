use crate::misc;
use std::collections::HashMap;

const BOX: u8 = 0;
const WALL: u8 = 1;
const UP: (i32, i32) = (-1, 0);
const DOWN: (i32, i32) = (1, 0);
const LEFT: (i32, i32) = (0, -1);
const RIGHT: (i32, i32) = (0, 1);

fn parse_board() -> (HashMap<(i32, i32), u8>, (i32, i32), Vec<(i32, i32)>) {
    let text = misc::text();
    let (board, movements) = text.split_once("\n\n").unwrap();
    let mut obstacles = HashMap::new();
    let mut robot = (0, 0);
    let movements = movements
        .chars()
        .filter_map(|char| match char {
            '^' => Some(UP),
            'v' => Some(DOWN),
            '<' => Some(LEFT),
            '>' => Some(RIGHT),
            _ => None,
        })
        .collect();

    board.split("\n").enumerate().for_each(|(y, row)| {
        row.chars().enumerate().for_each(|(x, char)| {
            let point = (y as i32, x as i32);
            match char {
                'O' => {
                    obstacles.insert(point, BOX);
                }
                '#' => {
                    obstacles.insert(point, WALL);
                }
                '@' => robot = point,
                _ => return,
            }
        });
    });

    (obstacles, robot, movements)
}

fn next_empty_space(
    obstacles: &HashMap<(i32, i32), u8>,
    robot: (i32, i32),
    direction: (i32, i32),
) -> Option<(i32, i32)> {
    for i in 1..100 {
        let point = (robot.0 + direction.0 * i, robot.1 + direction.1 * i);
        match obstacles.get(&point) {
            Some(&BOX) => continue,
            Some(&WALL) => break,
            _ => return Some(point),
        }
    }

    None
}

fn move_robot(
    obstacles: &mut HashMap<(i32, i32), u8>,
    robot: &mut (i32, i32),
    direction: (i32, i32),
) {
    let next_point = (robot.0 + direction.0, robot.1 + direction.1);
    match obstacles.get(&next_point) {
        Some(&BOX) => {
            if let Some(empty_point) = next_empty_space(obstacles, *robot, direction) {
                obstacles.remove(&next_point);
                obstacles.insert(empty_point, BOX);
                *robot = next_point;
            }
        }
        Some(&WALL) => {}
        _ => *robot = next_point,
    }
}

fn get_coordinates(obstacles: &HashMap<(i32, i32), u8>) -> usize {
    obstacles
        .iter()
        .filter(|(_, obstacle)| **obstacle == BOX)
        .map(|((y, x), _)| 100 * y + x)
        .sum::<i32>() as usize
}

pub fn a() -> usize {
    let (mut obstacles, mut robot, movements) = parse_board();
    movements.iter().for_each(|movement| {
        move_robot(&mut obstacles, &mut robot, *movement);
    });

    get_coordinates(&obstacles)
}

pub fn b() -> usize {
    0
}
