use crate::misc::{self, Point};
use std::collections::{HashMap, HashSet, VecDeque};

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

fn get_track(
    walls: &HashSet<Point>,
    start: &Point,
    end: &Point,
    dimensions: &Point,
) -> HashMap<Point, usize> {
    let mut queue = VecDeque::new();
    let mut visited = HashSet::new();
    queue.push_back((*start, 0, HashMap::new()));

    while let Some((point, cost, mut track)) = queue.pop_front() {
        if visited.contains(&point) {
            continue;
        }

        visited.insert(point);
        track.insert(point, cost);

        if point == *end {
            return track;
        }

        get_legal_moves(walls, dimensions, &point)
            .filter(|point| !visited.contains(point))
            .for_each(|point| queue.push_back((point, cost + 1, track.clone())));
    }

    HashMap::new()
}

fn solve(path: &str, cheat: i32, saved: usize) -> String {
    let (walls, start, end, dimensions) = parse(path);
    let track = get_track(&walls, &start, &end, &dimensions);
    let mut count = 0;

    for (start_point, start_idx) in track.iter() {
        for dy in -cheat..=cheat {
            for dx in (dy.abs() - cheat)..=(cheat - dy.abs()) {
                let end_point = (
                    (dy + start_point.0 as i32) as usize,
                    (dx + start_point.1 as i32) as usize,
                );

                if let Some(&end_idx) = track.get(&end_point) {
                    let distance = (dy.abs() + dx.abs()) as usize;
                    if end_idx >= start_idx + distance + saved {
                        count += 1;
                    }
                }
            }
        }
    }

    count.to_string()
}
pub fn a(path: &str) -> String {
    solve(path, 2, 100)
}

pub fn b(path: &str) -> String {
    solve(path, 20, 100)
}

#[test]
fn test() {
    assert!(a("input/exercise_20.txt") == "1327");
    assert!(b("input/exercise_20.txt") == "985737");
}
