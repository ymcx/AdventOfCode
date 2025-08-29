use crate::misc;
use std::collections::{HashMap, HashSet};

fn parse_rules(rules_str: &str) -> HashMap<&str, HashSet<&str>> {
    let mut rules = HashMap::new();
    rules_str.lines().for_each(|rule| {
        let (left, right) = rule.split_once("|").unwrap();
        rules.entry(left).or_insert_with(HashSet::new).insert(right);
    });

    rules
}

fn is_valid_update(update: &str, rules: &HashMap<&str, HashSet<&str>>) -> bool {
    let update_levels: Vec<_> = update.split(",").collect();
    for i in 1..update_levels.len() {
        let current_rules = rules.get(update_levels[i]).unwrap();
        for level in update_levels.iter().take(i) {
            if current_rules.contains(*level) {
                return false;
            }
        }
    }

    true
}

pub fn a() -> usize {
    let text = misc::text();
    let (rules, updates) = text.trim().split_once("\n\n").unwrap();
    let rules = parse_rules(rules);

    updates
        .lines()
        .map(|update| {
            if !is_valid_update(update, &rules) {
                return 0;
            }

            let levels: Vec<_> = update.split(",").collect();
            let middle = levels.len() / 2;
            levels[middle].parse().unwrap()
        })
        .sum()
}

pub fn b() -> usize {
    0
}
