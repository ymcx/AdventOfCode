use crate::misc;
use std::collections::HashMap;

fn parse_rules(rules_str: &str) -> HashMap<&str, Vec<&str>> {
    let mut rules = HashMap::new();
    rules_str.lines().for_each(|rule| {
        let (left, right) = rule.split_once("|").unwrap();
        rules.entry(left).or_insert_with(Vec::new).push(right);
    });

    rules
}

fn invalid_update(
    update: &Vec<String>,
    rules: &HashMap<&str, Vec<&str>>,
) -> Option<(usize, String)> {
    for i in 1..update.len() {
        let current_level = update[i].to_string();
        if let Some(current_rules) = rules.get(current_level.as_str()) {
            for level in update.iter().take(i) {
                if !current_rules.contains(&level.as_str()) {
                    continue;
                }

                let invalid_level = (i, current_level);
                return Some(invalid_level);
            }
        }
    }

    None
}

pub fn a(path: &str) -> String {
    let text = misc::text(path);
    let (rules, updates) = text.split_once("\n\n").unwrap();
    let rules = parse_rules(rules);

    updates
        .lines()
        .map(|update| {
            let update = update.split(",").map(String::from).collect();
            if invalid_update(&update, &rules).is_some() {
                return 0;
            }

            let middle = update.len() / 2;
            update[middle].parse().unwrap()
        })
        .sum::<usize>()
        .to_string()
}

pub fn b(path: &str) -> String {
    let text = misc::text(path);
    let (rules, updates) = text.split_once("\n\n").unwrap();
    let rules = parse_rules(rules);

    updates
        .lines()
        .map(|update| {
            let mut update = update.split(",").map(String::from).collect();
            let mut changed = false;

            while let Some((i, level)) = invalid_update(&update, &rules) {
                update.remove(i);
                update.insert(i - 1, level);
                changed = true;
            }

            if changed {
                let middle = update.len() / 2;
                return update[middle].parse().unwrap();
            }

            0
        })
        .sum::<usize>()
        .to_string()
}

#[test]
fn test() {
    assert!(a("input/exercise_5.txt") == "5391");
    assert!(b("input/exercise_5.txt") == "6142");
}
