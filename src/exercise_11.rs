use crate::misc;
use std::collections::HashMap;

fn do_blink(stones: &HashMap<usize, usize>) -> HashMap<usize, usize> {
    let mut stones_new = HashMap::new();
    for (&stone, &amount) in stones {
        if stone == 0 {
            *stones_new.entry(1).or_insert(0) += amount;
            continue;
        }

        let stone_str = stone.to_string();
        if stone_str.len() % 2 == 0 {
            let (l, r) = stone_str.split_at(stone_str.len() / 2);
            let (l, r) = (l.parse().unwrap(), r.parse().unwrap());
            *stones_new.entry(l).or_insert(0) += amount;
            *stones_new.entry(r).or_insert(0) += amount;
            continue;
        }

        *stones_new.entry(stone * 2024).or_insert(0) += amount;
    }

    stones_new
}

fn amount(path: &str, blinks: usize) -> usize {
    let text = misc::text(path);
    let mut stones = HashMap::new();

    text.split(" ").for_each(|num| {
        let num = num.parse().unwrap();
        *stones.entry(num).or_insert(0) += 1;
    });

    for _ in 0..blinks {
        stones = do_blink(&stones);
    }

    stones.values().sum()
}

pub fn a(path: &str) -> String {
    amount(path, 25).to_string()
}

pub fn b(path: &str) -> String {
    amount(path, 75).to_string()
}

#[test]
fn test() {
    assert!(a("input/exercise_11.txt") == "209412");
    assert!(b("input/exercise_11.txt") == "248967696501656");
}
