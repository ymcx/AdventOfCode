use crate::misc;
use std::collections::HashMap;

fn parse(path: &str) -> (Vec<String>, Vec<String>) {
    let text = misc::text(path);
    let (components, designs) = text.split_once("\n\n").unwrap();
    let components = components.split(", ").map(String::from).collect();
    let designs = designs.split("\n").map(String::from).collect();

    (components, designs)
}

fn check_design(
    components: &Vec<String>,
    cache: &mut HashMap<String, usize>,
    design: &str,
) -> usize {
    if let Some(&sum) = cache.get(design) {
        return sum;
    }

    if design.is_empty() {
        return 1;
    }

    let sum = components
        .iter()
        .filter(|component| design.starts_with(*component))
        .map(|component| check_design(components, cache, design.strip_prefix(component).unwrap()))
        .sum();

    cache.insert(design.to_string(), sum);

    sum
}

pub fn a(path: &str) -> String {
    let (components, designs) = parse(path);
    let mut cache = HashMap::new();

    designs
        .iter()
        .filter(|design| check_design(&components, &mut cache, design) != 0)
        .count()
        .to_string()
}

pub fn b(path: &str) -> String {
    let (components, designs) = parse(path);
    let mut cache = HashMap::new();

    designs
        .iter()
        .map(|design| check_design(&components, &mut cache, design))
        .sum::<usize>()
        .to_string()
}

#[test]
fn test() {
    assert!(a("input/exercise_19.txt") == "365");
    assert!(b("input/exercise_19.txt") == "730121486795169");
}
