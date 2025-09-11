use crate::misc::{self, Point};
use itertools::Itertools;
use std::collections::{HashSet, VecDeque};

fn parse_walls(path: &str, amount: usize) -> (HashSet<Point>, usize) {
    let walls: HashSet<Point> = misc::lines(path)
        .take(amount)
        .map(|line| {
            line.unwrap()
                .split(",")
                .map(|value| value.parse().unwrap())
                .collect_tuple()
                .unwrap()
        })
        .collect();
    let &(size, _) = walls.iter().max().unwrap();

    (walls, size)
}

fn calculate_cost(walls: &HashSet<Point>, size: usize) -> usize {
    let start = (0, 0);
    let end = (size, size);
    let mut queue = VecDeque::new();
    let mut visited = HashSet::new();
    queue.push_back((start, 0));

    while let Some((point, cost)) = queue.pop_front() {
        if visited.contains(&point) {
            continue;
        }

        if point == end {
            return cost;
        }

        visited.insert(point);

        [(0, 1), (-1, 0), (0, -1), (1, 0)]
            .iter()
            .map(|delta| {
                (
                    point.0.wrapping_add_signed(delta.0),
                    point.1.wrapping_add_signed(delta.1),
                )
            })
            .filter(|point| {
                !walls.contains(point)
                    && !visited.contains(point)
                    && point.0 <= size
                    && point.1 <= size
            })
            .for_each(|point| queue.push_back((point, cost + 1)));
    }

    0
}

pub fn a(path: &str) -> String {
    let walls_amount = 1024;
    let (walls, size) = parse_walls(path, walls_amount);
    calculate_cost(&walls, size).to_string()
}

pub fn b(_: &str) -> String {
    String::new()
}

#[test]
fn test() {
    assert!(a("input/exercise_18.txt") == "280");
    // assert!(b("input/exercise_18.txt") == "");
}
