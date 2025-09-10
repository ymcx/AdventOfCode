use crate::misc::{self, SignedPoint};
use itertools::Itertools;
use std::str::Split;

fn parse_values(parts: &mut Split<&str>) -> SignedPoint {
    parts.next().unwrap()[2..]
        .split(",")
        .map(|i| i.parse().unwrap())
        .collect_tuple()
        .unwrap()
}

fn parse_robots(path: &str) -> Vec<(SignedPoint, SignedPoint)> {
    misc::lines(path)
        .map(|line| {
            let line = line.unwrap();
            let mut parts = line.split(" ");
            (parse_values(&mut parts), parse_values(&mut parts))
        })
        .collect()
}

fn move_robots(
    robots: &mut Vec<(SignedPoint, SignedPoint)>,
    dimensions: SignedPoint,
    seconds: i32,
) {
    let (x_max, y_max) = dimensions;

    robots.iter_mut().for_each(|(position, velocity)| {
        *position = (
            (position.0 + velocity.0 * seconds).rem_euclid(x_max),
            (position.1 + velocity.1 * seconds).rem_euclid(y_max),
        );
    });
}

fn safe_value(robots: &Vec<(SignedPoint, SignedPoint)>, dimensions: SignedPoint) -> usize {
    let (x_max, y_max) = dimensions;
    let (x_mid, y_mid) = (x_max / 2, y_max / 2);

    let mut quadrants = vec![0; 4];
    robots.iter().for_each(|&((x, y), _)| {
        if x == x_mid || y == y_mid {
            return;
        }

        let mut quadrant = 0;
        if x < x_mid {
            quadrant += 1;
        }
        if y < y_mid {
            quadrant += 2;
        }
        quadrants[quadrant] += 1;
    });

    quadrants.iter().product()
}

fn contains_consecutive_columns(robots: &Vec<(SignedPoint, SignedPoint)>) -> bool {
    let positions: Vec<SignedPoint> = robots.iter().map(|&(position, _)| position).collect();
    positions
        .iter()
        .filter(|&&position| {
            vec![position; 7]
                .iter()
                .enumerate()
                .map(|(i, &(x, y))| (x, y + i as i32))
                .all(|position| positions.contains(&position))
        })
        .count()
        != 0
}

pub fn a(path: &str) -> usize {
    let mut robots = parse_robots(path);
    let dimensions = (101, 103);

    move_robots(&mut robots, dimensions, 100);
    safe_value(&robots, dimensions)
}

pub fn b(path: &str) -> usize {
    let mut robots = parse_robots(path);
    let dimensions = (101, 103);

    for i in 0..10000 {
        if contains_consecutive_columns(&robots) {
            return i;
        }

        move_robots(&mut robots, dimensions, 1);
    }

    0
}

#[test]
fn test() {
    assert!(a("input/exercise_14.txt") == 224554908);
    assert!(b("input/exercise_14.txt") == 6644);
}
