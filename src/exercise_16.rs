use crate::misc::{self, Point};
use std::collections::{HashMap, HashSet};

fn parse_walls(path: &str) -> (HashSet<Point>, Point, Point) {
    let mut walls = HashSet::new();
    let mut start = (0, 0);
    let mut end = (0, 0);
    for (y, row) in misc::lines(path).iter().enumerate() {
        for (x, char) in row.chars().enumerate() {
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

    (walls, start, end)
}

fn get_next_points(walls: &HashSet<Point>, point: Point) -> Vec<(usize, Point)> {
    [(0, 1), (-1, 0), (0, -1), (1, 0)]
        .into_iter()
        .enumerate()
        .filter_map(|(next_direction, delta)| {
            let next_point = (
                point.0.saturating_add_signed(delta.0),
                point.1.saturating_add_signed(delta.1),
            );

            if walls.contains(&next_point) {
                return None;
            }

            Some((next_direction, next_point))
        })
        .collect()
}

fn calculate_cost(walls: &HashSet<Point>, start: Point, end: Point) -> (usize, HashSet<Point>) {
    let mut players = vec![(start, 0, 0, HashSet::new())];
    let mut visited = HashMap::new();
    let mut min_cost = usize::MAX;
    let mut paths = HashSet::new();

    while let Some(mut player) = players.pop() {
        loop {
            let (point, direction, cost, mut path) = player.clone();

            if cost > min_cost {
                break;
            }

            if let Some(last_cost) = visited.get(&point) {
                if cost > last_cost + 1000 {
                    break;
                }
            }

            visited.insert(point, cost);
            path.insert(point);

            if point == end {
                if cost == min_cost {
                    paths.extend(path);
                } else {
                    min_cost = cost;
                    paths = path;
                }
                break;
            }

            let next_points = get_next_points(walls, point);
            if next_points.len() == 0 {
                break;
            }

            for (i, (next_direction, next_point)) in next_points.into_iter().enumerate() {
                let next_cost = cost + if direction == next_direction { 1 } else { 1001 };
                let next_path = path.clone();
                let next_player = (next_point, next_direction, next_cost, next_path);

                if i == 0 {
                    player = next_player;
                    continue;
                }

                players.push(next_player);
            }
        }
    }

    (min_cost, paths)
}

pub fn a(path: &str) -> String {
    let (walls, start, end) = parse_walls(path);
    let (cost, _) = calculate_cost(&walls, start, end);

    cost.to_string()
}

pub fn b(path: &str) -> String {
    let (walls, start, end) = parse_walls(path);
    let (_, paths) = calculate_cost(&walls, start, end);

    paths.len().to_string()
}

#[test]
fn test() {
    assert!(a("input/exercise_16.txt") == "101492");
    assert!(b("input/exercise_16.txt") == "543");
}
