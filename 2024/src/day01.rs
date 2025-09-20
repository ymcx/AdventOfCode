use crate::misc;

fn get_vectors(path: &str) -> (Vec<usize>, Vec<usize>) {
    let mut lv = Vec::new();
    let mut rv = Vec::new();

    misc::lines(path).iter().for_each(|line| {
        let (l, r) = line.split_at(5);
        let (l, r) = (l.parse().unwrap(), r.trim().parse().unwrap());
        lv.push(l);
        rv.push(r);
    });

    (lv, rv)
}

pub fn a(path: &str) -> String {
    let (mut lv, mut rv) = get_vectors(path);

    lv.sort();
    rv.sort();

    lv.into_iter()
        .zip(rv.into_iter())
        .map(|(l, r)| l.abs_diff(r))
        .sum::<usize>()
        .to_string()
}

pub fn b(path: &str) -> String {
    let (lv, rv) = get_vectors(path);

    lv.into_iter()
        .map(|l| l * rv.iter().filter(|r| **r == l).count())
        .sum::<usize>()
        .to_string()
}
