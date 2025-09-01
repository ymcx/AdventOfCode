use crate::misc;
use std::collections::{HashMap, HashSet};

fn parse_board() -> (HashMap<char, HashSet<(i32, i32)>>, (i32, i32)) {
    let mut antennas = HashMap::new();
    let lines: Vec<String> = misc::lines().map(|i| i.unwrap()).collect();
    let dimensions = (lines.len() as i32, lines[0].len() as i32);

    lines.iter().enumerate().for_each(|(y, line)| {
        line.chars().enumerate().for_each(|(x, char)| {
            if char == '.' {
                return;
            }

            let position = (y as i32, x as i32);
            antennas
                .entry(char)
                .or_insert_with(HashSet::new)
                .insert(position);
        });
    });

    (antennas, dimensions)
}

pub fn a() -> usize {
    let (antennas, dimensions) = parse_board();
    let (y_max, x_max) = dimensions;

    let mut antidotes = HashSet::new();
    for (_, antennas) in antennas {
        for first in antennas.iter() {
            for second in antennas.iter() {
                if first == second {
                    continue;
                }

                let antidote = (2 * first.0 - second.0, 2 * first.1 - second.1);
                antidotes.insert(antidote);
                let antidote = (2 * second.0 - first.0, 2 * second.1 - first.1);
                antidotes.insert(antidote);
            }
        }
    }

    antidotes
        .into_iter()
        .filter(|&(y, x)| y >= 0 && x >= 0 && y_max > y && x_max > x)
        .count()
}

pub fn b() -> usize {
    let (antennas, dimensions) = parse_board();
    let (y_max, x_max) = dimensions;

    let mut antidotes = HashSet::new();
    for (_, antennas) in antennas {
        for first in antennas.iter() {
            for second in antennas.iter() {
                if first == second {
                    continue;
                }

                let delta = (second.0 - first.0, second.1 - first.1);
                let mut antidote = *second;
                let (mut y, mut x) = antidote;
                while y >= 0 && x >= 0 && y_max > y && x_max > x {
                    antidotes.insert(antidote);
                    antidote = (y + delta.0, x + delta.1);
                    (y, x) = antidote;
                }
            }
        }
    }

    antidotes.len()
}
