use crate::misc;

fn parse(path: &str) -> (Vec<[usize; 5]>, Vec<[usize; 5]>) {
    let (mut locks, mut keys) = (Vec::new(), Vec::new());

    misc::text(path).split("\n\n").for_each(|schematic| {
        let mut item = [0; 5];
        for line in schematic.lines().skip(1).take(5) {
            for (i, char) in line.chars().enumerate() {
                if char == '#' {
                    item[i] += 1;
                }
            }
        }

        let is_lock = schematic.chars().next().unwrap() == '#';
        if is_lock {
            locks.push(item);
        } else {
            keys.push(item);
        }
    });

    (locks, keys)
}

pub fn a(path: &str) -> String {
    let (locks, keys) = parse(path);

    locks
        .iter()
        .map(|lock| {
            keys.iter()
                .filter(|key| (0..5).all(|i| key[i] + lock[i] <= 5))
                .count()
        })
        .sum::<usize>()
        .to_string()
}
