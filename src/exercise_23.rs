use crate::misc;
use petgraph::{algo::maximal_cliques, graph::UnGraph};
use std::collections::{HashMap, HashSet};

fn get_graph(path: &str) -> UnGraph<String, ()> {
    let mut graph = UnGraph::new_undirected();
    let mut nodes = HashMap::new();

    for line in misc::lines(path).iter() {
        let (left, right) = line.split_once('-').unwrap();
        let (left, right) = (
            *nodes
                .entry(left)
                .or_insert(graph.add_node(left.to_string())),
            *nodes
                .entry(right)
                .or_insert(graph.add_node(right.to_string())),
        );

        graph.add_edge(left, right, ());
    }

    graph
}

fn solve(graph: &UnGraph<String, ()>) -> (usize, String) {
    let mut triplets = HashSet::new();
    for i in graph.node_indices() {
        if !graph[i].starts_with('t') {
            continue;
        }

        let neighbors: Vec<_> = graph.neighbors(i).collect();
        for a in 0..neighbors.len() {
            for b in (a + 1)..neighbors.len() {
                let a = neighbors[a];
                let b = neighbors[b];

                if graph.contains_edge(a, b) {
                    let mut triplet = vec![i, a, b];
                    triplet.sort();
                    triplets.insert(triplet);
                }
            }
        }
    }

    let mut longest_set: Vec<_> = maximal_cliques(graph)
        .iter()
        .max_by_key(|i| i.len())
        .unwrap()
        .iter()
        .map(|&i| graph[i].to_string())
        .collect();
    longest_set.sort();

    (triplets.len(), longest_set.join(","))
}

pub fn a(path: &str) -> String {
    let graph = get_graph(path);
    let (sets, _) = solve(&graph);

    sets.to_string()
}

pub fn b(path: &str) -> String {
    let graph = get_graph(path);
    let (_, password) = solve(&graph);

    password
}

#[test]
fn test() {
    assert!(a("input/exercise_23.txt") == "1149");
    assert!(b("input/exercise_23.txt") == "as,co,do,kh,km,mc,np,nt,un,uq,wc,wz,yo");
}
