use crate::misc;

pub fn a() -> usize {
    let text = misc::text();
    let text: Vec<i32> = text
        .trim()
        .chars()
        .enumerate()
        .map(|(i, char)| {
            let value = if i % 2 == 0 { i as i32 / 2 } else { -1 };
            let length = char.to_digit(10).unwrap() as usize;
            vec![value; length]
        })
        .flatten()
        .collect();

    let mut last = text.len() - 1;
    let text: Vec<i32> = text
        .iter()
        .enumerate()
        .map(|(i, &char)| {
            if i > last {
                return -1;
            }

            if char != -1 {
                return char;
            }

            while text[last] == -1 {
                last -= 1;
            }
            last -= 1;
            text[last + 1]
        })
        .collect();

    text.iter()
        .enumerate()
        .filter(|&(_, &char)| char != -1)
        .map(|(i, &char)| i * char as usize)
        .sum()
}

pub fn b() -> usize {
    0
}
