use crate::misc;

fn get_vectors() -> (Vec<usize>, Vec<usize>) {
    let mut lv = Vec::new();
    let mut rv = Vec::new();

    misc::lines().for_each(|line| {
        let line = line.unwrap();
        let (l, r) = line.split_at(5);
        let (l, r) = (l.parse().unwrap(), r.trim().parse().unwrap());
        lv.push(l);
        rv.push(r);
    });

    (lv, rv)
}

pub fn a() -> usize {
    let (mut lv, mut rv) = get_vectors();

    lv.sort();
    rv.sort();

    lv.into_iter()
        .zip(rv.into_iter())
        .map(|(l, r)| l.abs_diff(r))
        .sum()
}

pub fn b() -> usize {
    let (lv, rv) = get_vectors();

    lv.into_iter()
        .map(|l| l * rv.iter().filter(|r| **r == l).count())
        .sum()
}
