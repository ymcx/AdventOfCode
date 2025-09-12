use crate::misc::{self, Point};
use std::collections::{HashSet, VecDeque};

fn parse(path: &str) -> (HashSet<Point>, Point, Point, Point) {
    let lines = misc::lines(path);
    let mut walls = HashSet::new();
    let mut start = (0, 0);
    let mut end = (0, 0);
    let dimensions = (lines.len(), lines[0].len());

    for (y, line) in lines.iter().enumerate() {
        for (x, char) in line.chars().enumerate() {
            let point = (y, x);
            match char {
                '#' => {
                    walls.insert(point);
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

    (walls, start, end, dimensions)
}

fn get_legal_moves(
    walls: &HashSet<Point>,
    dimensions: &Point,
    point: &Point,
) -> impl Iterator<Item = Point> {
    [(0, 1), (-1, 0), (0, -1), (1, 0)]
        .iter()
        .map(|delta| {
            (
                point.0.wrapping_add_signed(delta.0),
                point.1.wrapping_add_signed(delta.1),
            )
        })
        .filter(|point| {
            !walls.contains(point) && point.0 <= dimensions.0 && point.1 <= dimensions.1
        })
}

fn get_time(walls: &HashSet<Point>, start: &Point, end: &Point, dimensions: &Point) -> usize {
    let mut queue = VecDeque::new();
    let mut visited = HashSet::new();
    let mut total_cost = 0;
    queue.push_back((*start, 0));

    while let Some((point, cost)) = queue.pop_front() {
        if visited.contains(&point) {
            continue;
        }

        if point == *end {
            total_cost = cost;
            break;
        }

        visited.insert(point);

        get_legal_moves(walls, dimensions, &point)
            .filter(|point| !visited.contains(point))
            .for_each(|point| queue.push_back((point, cost + 1)));
    }

    total_cost
}

fn wall_combinations(walls: &HashSet<Point>, dimensions: &Point) -> Vec<HashSet<Point>> {
    walls
        .iter()
        .filter(|wall| get_legal_moves(walls, dimensions, wall).count() != 0)
        .map(|wall| {
            let mut combination = walls.clone();
            combination.remove(wall);
            combination
        })
        .collect()
}

pub fn a(path: &str) -> String {
    let (walls, start, end, dimensions) = parse(path);
    let legit_time = get_time(&walls, &start, &end, &dimensions);

    wall_combinations(&walls, &dimensions)
        .iter()
        .filter(|walls| 100 <= legit_time - get_time(&walls, &start, &end, &dimensions))
        .count()
        .to_string()
}

pub fn b(_path: &str) -> String {
    "".to_string()
}

#[test]
fn test() {
    assert!(a("input/exercise_20.txt") == "1327");
    // assert!(b("input/exercise_20.txt") != 0);
}
