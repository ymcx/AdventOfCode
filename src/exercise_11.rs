use crate::misc;

fn do_blink(stones: &Vec<usize>) -> Vec<usize> {
    let mut new_stones = Vec::new();
    for i in 0..stones.len() {
        let stone = stones[i];
        let stone_str = stone.to_string();
        let some_stones = if stone == 0 {
            vec![1]
        } else if stone_str.len() % 2 == 0 {
            let (l, r) = stone_str.split_at(stone_str.len() / 2);
            let (l, r) = (l.parse().unwrap(), r.parse().unwrap());
            vec![l, r]
        } else {
            vec![stone * 2024]
        };

        new_stones.extend(some_stones);
    }

    new_stones
}

pub fn a() -> usize {
    let text = misc::text();
    let mut stones = text.trim().split(" ").map(|i| i.parse().unwrap()).collect();

    for _ in 0..25 {
        stones = do_blink(&stones);
    }

    stones.len()
}

pub fn b() -> usize {
    0
}
