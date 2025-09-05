use crate::misc;
use std::{iter::Skip, str::Split};

fn parse_numbers(
    components: &mut Skip<Split<&str>>,
    first_trim: (&str, &str),
    second_trim: (&str, &str),
) -> (i32, i32) {
    (
        components
            .next()
            .unwrap()
            .trim_start_matches(first_trim.0)
            .trim_end_matches(first_trim.1)
            .parse()
            .unwrap(),
        components
            .next()
            .unwrap()
            .trim_start_matches(second_trim.0)
            .trim_end_matches(second_trim.1)
            .parse()
            .unwrap(),
    )
}

fn parse_machine(machine: &str) -> ((i32, i32), (i32, i32), (i32, i32)) {
    let mut parts = machine.split("\n").into_iter();
    let (mut a, mut b, mut prize) = (
        parts.next().unwrap().split(" ").skip(2),
        parts.next().unwrap().split(" ").skip(2),
        parts.next().unwrap().split(" ").skip(1),
    );
    (
        parse_numbers(&mut a, ("X", ","), ("Y", "")),
        parse_numbers(&mut b, ("X", ","), ("Y", "")),
        parse_numbers(&mut prize, ("X=", ","), ("Y=", "")),
    )
}

fn gcd(a: i32, b: i32) -> (i32, i32, i32) {
    if b == 0 {
        return (a, 1, 0);
    }

    let (d, x, y) = gcd(b, a % b);
    (d, y, x - (a / b) * y)
}

fn find_solutions(a: i32, b: i32, c: i32) -> Vec<(i32, i32)> {
    let (d, x, y) = gcd(a, b);
    if c % d != 0 {
        return Vec::new();
    }

    let x = x * c / d;
    let y = y * c / d;
    let step_x = b / d;
    let step_y = a / d;
    let min = -x / step_x;
    let max = y / step_y;

    (min..=max)
        .map(|t| (x + step_x * t, y - step_y * t))
        .collect()
}

pub fn a() -> usize {
    let cost = (3, 1);
    let machines: Vec<_> = misc::text()
        .split("\n\n")
        .map(|machine| parse_machine(machine))
        .collect();

    machines
        .iter()
        .map(|machine| {
            let (a, b, prize) = machine;
            let x: Vec<_> = find_solutions(a.0, b.0, prize.0)
                .into_iter()
                .filter(|(x, y)| a.1 * x + b.1 * y == prize.1)
                .collect();

            if x.is_empty() {
                return 0;
            }

            x.into_iter()
                .map(|(a, b)| (cost.0 * a + cost.1 * b) as usize)
                .min()
                .unwrap()
        })
        .sum()
}

pub fn b() -> usize {
    0
}
