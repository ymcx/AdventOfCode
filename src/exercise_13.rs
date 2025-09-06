use crate::misc;
use std::{iter::Skip, str::Split};

fn parse_numbers(
    components: &mut Skip<Split<&str>>,
    first_trim: (&str, &str),
    second_trim: (&str, &str),
) -> (isize, isize) {
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

fn parse_machine(machine: &str) -> ((isize, isize), (isize, isize), (isize, isize)) {
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

fn gcd(a: isize, b: isize) -> (isize, isize, isize) {
    if b == 0 {
        return (a, 1, 0);
    }

    let (d, x, y) = gcd(b, a % b);
    (d, y, x - (a / b) * y)
}

fn find_solution(
    a: (isize, isize),
    b: (isize, isize),
    prize: (isize, isize),
    cost: (isize, isize),
) -> usize {
    let (d, x, y) = gcd(a.0, b.0);
    let x = x * prize.0 / d;
    let y = y * prize.0 / d;
    let step_x = b.0 / d;
    let step_y = a.0 / d;

    let numerator = prize.1 - a.1 * x - b.1 * y;
    let denominator = a.1 * step_x - b.1 * step_y;
    if numerator % denominator != 0 {
        return 0;
    }

    let x = x + step_x * numerator / denominator;
    let y = y - step_y * numerator / denominator;
    (cost.0 * x + cost.1 * y) as usize
}

fn total_cost(offset: isize, cost: (isize, isize)) -> usize {
    misc::text()
        .split("\n\n")
        .map(|machine| {
            let (a, b, prize) = parse_machine(machine);
            let prize = (prize.0 + offset, prize.1 + offset);
            find_solution(a, b, prize, cost)
        })
        .sum()
}

pub fn a() -> usize {
    let offset = 0;
    let cost = (3, 1);
    total_cost(offset, cost)
}

pub fn b() -> usize {
    let offset = isize::pow(10, 13);
    let cost = (3, 1);
    total_cost(offset, cost)
}
