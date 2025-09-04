use crate::misc;
use std::collections::{HashMap, HashSet, VecDeque};

fn groups(points: &HashSet<(i32, i32)>) -> usize {
    let mut unvisited = points.clone();
    let mut groups = 0;

    while let Some(&point) = unvisited.iter().next() {
        let mut queue = VecDeque::new();
        queue.push_back(point);
        unvisited.remove(&point);
        groups += 1;

        while let Some(point) = queue.pop_front() {
            let (y, x) = point;
            for new_point in [(y + 1, x), (y - 1, x), (y, x + 1), (y, x - 1)] {
                if unvisited.remove(&new_point) {
                    queue.push_back(new_point);
                }
            }
        }
    }

    groups
}

fn travel(
    board: &Vec<Vec<char>>,
    travelled: &mut HashSet<(i32, i32)>,
    borders: &mut HashMap<usize, HashSet<(i32, i32)>>,
    dimensions: (i32, i32),
    last_char: char,
    point: (i32, i32),
    direction: usize,
) {
    if travelled.contains(&point) {
        return;
    }

    let (y, x) = point;
    let (y_max, x_max) = dimensions;
    let out_of_bounds = y < 0 || x < 0 || y >= y_max || x >= x_max;
    if out_of_bounds {
        borders
            .entry(direction)
            .or_insert(HashSet::new())
            .insert(point);
        return;
    }

    let new_char = board[y as usize][x as usize];
    if new_char != last_char {
        borders
            .entry(direction)
            .or_insert(HashSet::new())
            .insert(point);
        return;
    }

    travelled.insert(point);
    [(y + 1, x), (y - 1, x), (y, x + 1), (y, x - 1)]
        .iter()
        .enumerate()
        .for_each(|(new_direction, &new_point)| {
            travel(
                board,
                travelled,
                borders,
                dimensions,
                last_char,
                new_point,
                new_direction,
            )
        });
}

fn parse_board() -> (Vec<Vec<char>>, (i32, i32)) {
    let board: Vec<Vec<char>> = misc::lines()
        .map(|line| line.unwrap().chars().collect())
        .collect();
    let dimensions = (board.len() as i32, board[0].len() as i32);

    (board, dimensions)
}

fn price(first_exercise: bool) -> usize {
    let (board, dimensions) = parse_board();
    let (y_max, x_max) = dimensions;
    let mut total_travelled = HashSet::new();
    let mut price = 0;

    for y in 0..y_max {
        for x in 0..x_max {
            let char = board[y as usize][x as usize];
            let point = (y, x);
            if total_travelled.contains(&point) {
                continue;
            }

            let mut travelled = HashSet::new();
            let mut borders = HashMap::new();
            travel(
                &board,
                &mut travelled,
                &mut borders,
                dimensions,
                char,
                point,
                0,
            );

            let perimeter: usize = borders
                .iter()
                .map(|(_, i)| if first_exercise { i.len() } else { groups(i) })
                .sum();
            let area = travelled.len();
            total_travelled.extend(travelled);
            price += perimeter * area;
        }
    }

    price
}

pub fn a() -> usize {
    price(true)
}

pub fn b() -> usize {
    price(false)
}
