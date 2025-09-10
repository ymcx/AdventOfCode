use crate::misc;

pub fn a(path: &str) -> usize {
    let text = misc::text(path);
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

pub fn b(path: &str) -> usize {
    let text = misc::text(path);
    let mut text: Vec<(i32, usize)> = text
        .trim()
        .chars()
        .enumerate()
        .map(|(i, char)| {
            let value = if i % 2 == 0 { i as i32 / 2 } else { -1 };
            let length = char.to_digit(10).unwrap() as usize;
            (value, length)
        })
        .collect();

    let mut i = text.len();
    while i > 0 {
        i -= 1;

        let (value, length) = text[i];
        if value == -1 {
            continue;
        }

        for j in 0..i {
            let (slot_value, slot_length) = text[j];
            if slot_value == -1 && slot_length >= length {
                text[j] = (value, length);
                text[i] = (-1, length);
                if slot_length > length {
                    let new_slot = (-1, slot_length - length);
                    text.insert(j + 1, new_slot);
                }
                break;
            }
        }
    }

    let mut i = 0;
    text.iter()
        .map(|&(value, length)| {
            if value == -1 {
                i += length;
                return 0;
            }

            (0..length)
                .map(|_| {
                    let j = i * value as usize;
                    i += 1;
                    j
                })
                .sum()
        })
        .sum()
}

#[test]
fn test() {
    assert!(a("input/exercise_9.txt") == 6378826667552);
    assert!(b("input/exercise_9.txt") == 6413328569890);
}
