use crate::misc;

fn parse_machine(path: &str) -> (usize, Vec<usize>) {
    let text = misc::text(path);
    let (registers, instructions) = text.split_once("\n\n").unwrap();
    let register = registers
        .lines()
        .next()
        .unwrap()
        .chars()
        .filter(|c| c.is_ascii_digit())
        .collect::<String>()
        .parse()
        .unwrap();
    let instructions = instructions
        .chars()
        .filter(|c| c.is_ascii_digit())
        .map(|c| c.to_digit(10).unwrap() as usize)
        .rev()
        .collect();

    (register, instructions)
}

// https://blog.lojic.com/2024/12/17/advent-of-code-2024-day-17-chronospatial-computer.html
fn run(a: usize) -> (usize, usize) {
    let output = (a % 8 ^ 5 ^ a / 2usize.pow(a as u32 % 8 ^ 5) ^ 6) % 8;
    (output, a / 8)
}

fn check(m: usize, i: usize, expected: &Vec<usize>) -> Vec<usize> {
    (m..m + 8)
        .filter_map(|a| {
            let (output, _) = run(a);
            if output == expected[i] {
                if i == 15 {
                    return Some(vec![a]);
                }
                let m = a * 8;
                let i = i + 1;
                return Some(check(m, i, expected));
            }
            None
        })
        .flatten()
        .collect()
}

pub fn a(path: &str) -> String {
    let (mut a, _) = parse_machine(path);
    let mut output = 0;
    (0..)
        .map_while(|_| {
            if a == 0 {
                return None;
            }
            (output, a) = run(a);
            Some(output.to_string())
        })
        .collect::<Vec<String>>()
        .join(",")
}

pub fn b(path: &str) -> String {
    let (_, instructions) = parse_machine(path);
    check(0, 0, &instructions)[0].to_string()
}
