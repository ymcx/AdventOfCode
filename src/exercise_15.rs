use crate::misc;
use std::collections::{HashMap, HashSet, VecDeque};

const BOX: u8 = 0;
const WALL: u8 = 1;
const BOX_R: u8 = 2;
const UP: (i32, i32) = (-1, 0);
const DOWN: (i32, i32) = (1, 0);
const LEFT: (i32, i32) = (0, -1);
const RIGHT: (i32, i32) = (0, 1);

fn parse_board(large_board: bool) -> (HashMap<(i32, i32), u8>, (i32, i32), Vec<(i32, i32)>) {
    let mut text = misc::text();
    if large_board {
        text = text
            .replace("#", "##")
            .replace("O", "[]")
            .replace(".", "..")
            .replace("@", "@.");
    }

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
                'O' | '[' => {
                    obstacles.insert(point, BOX);
                }
                ']' => {
                    obstacles.insert(point, BOX_R);
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
            Some(&BOX) | Some(&BOX_R) => continue,
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

fn move_robot_large(
    obstacles: &mut HashMap<(i32, i32), u8>,
    point: &mut (i32, i32),
    direction: (i32, i32),
) {
    let next = (point.0 + direction.0, point.1 + direction.1);
    if let Some(next) = match obstacles.get(&next) {
        Some(&WALL) => None,
        Some(&BOX) | Some(&BOX_R) => move_boxes(obstacles, next, direction),
        _ => Some(next),
    } {
        *point = next;
    }
}

fn get_some_boxes_vertical(
    obstacles: &HashMap<(i32, i32), u8>,
    point: (i32, i32),
    direction: (i32, i32),
) -> Option<HashSet<(i32, i32)>> {
    let center = (point.0 + direction.0, point.1 + direction.1);
    let right = (center.0, center.1 + 1);
    let left = (center.0, center.1 - 1);

    let center_type = obstacles.get(&center);
    let right_type = obstacles.get(&right);

    if center_type == Some(&WALL) || right_type == Some(&WALL) {
        return None;
    }

    let mut boxes = HashSet::new();
    match center_type {
        Some(&BOX) => boxes.insert(center),
        Some(&BOX_R) => boxes.insert(left),
        _ => true,
    };

    if right_type == Some(&BOX) {
        boxes.insert(right);
    }

    Some(boxes)
}

fn get_boxes_vertical(
    obstacles: &HashMap<(i32, i32), u8>,
    point: (i32, i32),
    direction: (i32, i32),
) -> HashSet<(i32, i32)> {
    let mut boxes = HashSet::new();
    let mut queue = VecDeque::new();
    queue.push_back(point);

    while let Some(point) = queue.pop_front() {
        boxes.insert(point);

        if let Some(boxes) = get_some_boxes_vertical(obstacles, point, direction) {
            queue.extend(boxes);
            continue;
        }

        return HashSet::new();
    }

    boxes
}

fn get_boxes_horizontal(
    obstacles: &HashMap<(i32, i32), u8>,
    point: (i32, i32),
    direction: (i32, i32),
) -> HashSet<(i32, i32)> {
    let mut boxes = HashSet::new();
    boxes.insert(point);

    let mut point = point;
    let mut point_type = Some(&BOX);

    while point_type == Some(&BOX) || point_type == Some(&BOX_R) {
        point = (point.0 + direction.0, point.1 + direction.1);
        point_type = obstacles.get(&point);

        if point_type == Some(&BOX) {
            boxes.insert(point);
        }
    }

    if point_type == Some(&WALL) {
        return HashSet::new();
    }

    boxes
}

fn get_left_side(obstacles: &HashMap<(i32, i32), u8>, point: (i32, i32)) -> (i32, i32) {
    if obstacles.get(&point) == Some(&BOX) {
        point
    } else {
        (point.0, point.1 - 1)
    }
}

fn move_boxes(
    obstacles: &mut HashMap<(i32, i32), u8>,
    point: (i32, i32),
    direction: (i32, i32),
) -> Option<(i32, i32)> {
    let left = get_left_side(obstacles, point);
    let boxes = if direction == UP || direction == DOWN {
        get_boxes_vertical(obstacles, left, direction)
    } else {
        get_boxes_horizontal(obstacles, left, direction)
    };

    if boxes.is_empty() {
        return None;
    }

    for left in boxes.iter() {
        let right = &(left.0, left.1 + 1);
        obstacles.remove(left);
        obstacles.remove(right);
    }

    for left in boxes.iter() {
        let next_left = (left.0 + direction.0, left.1 + direction.1);
        let next_right = (next_left.0, next_left.1 + 1);
        obstacles.insert(next_left, BOX);
        obstacles.insert(next_right, BOX_R);
    }

    Some(point)
}

fn get_coordinates(obstacles: &HashMap<(i32, i32), u8>) -> usize {
    obstacles
        .iter()
        .filter(|(_, obstacle)| **obstacle == BOX)
        .map(|((y, x), _)| 100 * y + x)
        .sum::<i32>() as usize
}

pub fn a() -> usize {
    let (mut obstacles, mut robot, movements) = parse_board(false);
    movements.iter().for_each(|movement| {
        move_robot(&mut obstacles, &mut robot, *movement);
    });

    get_coordinates(&obstacles)
}

pub fn b() -> usize {
    let (mut obstacles, mut robot, movements) = parse_board(true);
    movements.iter().for_each(|movement| {
        move_robot_large(&mut obstacles, &mut robot, *movement);
    });

    get_coordinates(&obstacles)
}
