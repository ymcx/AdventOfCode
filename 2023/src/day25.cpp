#include "misc/io.h"
#include "misc/string.h"
#include <bits/stdc++.h>

using namespace std;

unordered_map<int, unordered_set<int>> parse_components(const string &path) {
  const vector<string> lines = read_lines(path);
  unordered_map<int, unordered_set<int>> components;
  unordered_map<string, int> id;
  int i = 0;

  for (const string &line : lines) {
    const vector<string> parts = split_colonspace(line);
    const string key = parts[0];
    const vector<string> values = split_space(parts[1]);

    if (!id.contains(key)) {
      id[key] = i++;
    }

    for (const string &value : values) {
      if (!id.contains(value)) {
        id[value] = i++;
      }

      int k = id[key];
      int v = id[value];

      components[k].insert(v);
      components[v].insert(k);
    }
  }

  return components;
}

pair<vector<vector<int>>, vector<int>>
sizes_after_removal(const int &n, const vector<vector<int>> &adjacent,
                    const vector<int> &removed) {
  vector<char> blocked(n, 0);
  vector<char> seen(n, 0);

  for (const int &i : removed) {
    blocked[i] = 1;
  }

  vector<vector<int>> components;
  vector<int> comp_of(n, -1);
  queue<int> queue;

  for (int i = 0; i < n; ++i) {
    if (blocked[i] || seen[i]) {
      continue;
    }

    const int cid = components.size();
    components.emplace_back();
    seen[i] = 1;
    queue.push(i);
    while (!queue.empty()) {
      int u = queue.front();
      queue.pop();
      components[cid].push_back(u);
      comp_of[u] = cid;
      for (int v : adjacent[u])
        if (!blocked[v] && !seen[v]) {
          seen[v] = 1;
          queue.push(v);
        }
    }
    sort(components[cid].begin(), components[cid].end());
  }

  return {components, comp_of};
}

vector<int> assign_removed(int n, const vector<vector<int>> &adjacent,
                           const vector<vector<int>> &components,
                           const vector<int> &comp_of,
                           const vector<int> &removed) {
  unordered_set<int> rem(removed.begin(), removed.end());
  vector<int> assign;
  assign.reserve(removed.size());
  for (int rm : removed) {
    unordered_map<int, int> cnt;
    for (int nb : adjacent[rm])
      if (!rem.count(nb) && comp_of[nb] != -1)
        ++cnt[comp_of[nb]];
    if (!cnt.empty()) {
      int best = -1, bc = -1;
      for (auto &p : cnt)
        if (p.second > bc ||
            (p.second == bc && (best == -1 || p.first < best))) {
          best = p.first;
          bc = p.second;
        }
      assign.push_back(best);
      continue;
    }

    vector<char> seen(n, 0);
    queue<int> q;
    unordered_set<int> found;
    for (int nb : adjacent[rm])
      if (!rem.count(nb) && !seen[nb]) {
        seen[nb] = 1;
        q.push(nb);
      }
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      if (comp_of[u] != -1)
        found.insert(comp_of[u]);
      for (int v : adjacent[u])
        if (!rem.count(v) && !seen[v]) {
          seen[v] = 1;
          q.push(v);
        }
    }
    if (!found.empty()) {
      assign.push_back(*min_element(found.begin(), found.end()));
      continue;
    }
    assign.push_back(components.empty() ? -1 : 0);
  }
  return assign;
}

/* Compact Dinic */
struct Dinic {
  struct E {
    int to, rev, cap;
  };
  vector<vector<E>> g;
  vector<int> lvl, it;
  Dinic(int n = 0) : g(n), lvl(n), it(n) {}
  void addEdge(int u, int v, int c) {
    g[u].push_back({v, (int)g[v].size(), c});
    g[v].push_back({u, (int)g[u].size() - 1, 0});
  }
  bool bfs(int s, int t) {
    fill(lvl.begin(), lvl.end(), -1);
    queue<int> q;
    lvl[s] = 0;
    q.push(s);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (auto &e : g[u])
        if (e.cap > 0 && lvl[e.to] < 0) {
          lvl[e.to] = lvl[u] + 1;
          q.push(e.to);
        }
    }
    return lvl[t] >= 0;
  }
  int dfs(int u, int t, int f) {
    if (u == t)
      return f;
    for (int &i = it[u]; i < (int)g[u].size(); ++i) {
      auto &e = g[u][i];
      if (e.cap > 0 && lvl[e.to] == lvl[u] + 1) {
        int got = dfs(e.to, t, min(f, e.cap));
        if (got) {
          e.cap -= got;
          g[e.to][e.rev].cap += got;
          return got;
        }
      }
    }
    return 0;
  }
  int maxflow(int s, int t) {
    int flow = 0, INF = 1e9;
    while (bfs(s, t)) {
      fill(it.begin(), it.end(), 0);
      while (int pushed = dfs(s, t, INF))
        flow += pushed;
    }
    return flow;
  }
  vector<char> reachable_from(int s) {
    vector<char> seen(g.size(), 0);
    queue<int> q;
    seen[s] = 1;
    q.push(s);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (auto &e : g[u])
        if (e.cap > 0 && !seen[e.to]) {
          seen[e.to] = 1;
          q.push(e.to);
        }
    }
    return seen;
  }
};

pair<int, vector<int>>
min_vertex_cut_for_pair(int n, const vector<vector<int>> &adjacent, int s,
                        int t) {
  const int INF = 1'000'000'000;
  Dinic D(2 * n);
  for (int v = 0; v < n; ++v)
    D.addEdge(2 * v, 2 * v + 1, (v == s || v == t) ? INF : 1);
  for (int u = 0; u < n; ++u)
    for (int v : adjacent[u])
      D.addEdge(2 * u + 1, 2 * v, INF);
  int src = 2 * s + 1, sink = 2 * t;
  int flow = D.maxflow(src, sink);
  auto reach = D.reachable_from(src);
  vector<int> cut;
  for (int v = 0; v < n; ++v)
    if (reach[2 * v] && !reach[2 * v + 1])
      cut.push_back(v);
  return {flow, cut};
}

pair<int, vector<int>> minimum_vertex_cut(int n,
                                          const vector<vector<int>> &adjacent) {
  vector<char> vis(n, 0);
  function<void(int)> dfs = [&](int u) {
    vis[u] = 1;
    for (int v : adjacent[u])
      if (!vis[v])
        dfs(v);
  };
  dfs(0);
  for (int i = 0; i < n; ++i)
    if (!vis[i])
      return {0, {}};
  int best = INT_MAX;
  vector<int> best_cut;
  for (int t = 1; t < n; ++t) {
    auto [flow, cut] = min_vertex_cut_for_pair(n, adjacent, 0, t);
    if (flow < best) {
      best = flow;
      best_cut = cut;
      if (best == 1)
        break;
    }
  }
  return {best, best_cut};
}

int main(int argc, char **argv) {
  auto components_map = parse_components(argv[1]);
  int n = components_map.size();
  vector<vector<int>> adjacent(n);
  for (auto &kv : components_map)
    for (int v : kv.second) {
      adjacent[kv.first].push_back(v);
      adjacent[v].push_back(kv.first);
    }
  auto [k, cut] = minimum_vertex_cut(n, adjacent);

  sort(cut.begin(), cut.end());
  auto [components, comp_of] = sizes_after_removal(n, adjacent, cut);
  auto assign = assign_removed(n, adjacent, components, comp_of, cut);

  vector<int> sizes;
  for (auto &c : components)
    sizes.push_back((int)c.size());
  for (int a : assign) {
    if (a >= 0 && a < (int)sizes.size())
      sizes[a]++;
    else
      sizes.push_back(1);
  }
  if (sizes.size() < 2)
    sizes.resize(2, 0);
  sort(sizes.begin(), sizes.end(), greater<int>());
  int result = sizes[0] * sizes[1];
  assert_print(result, 598120, result, 598120);
  return 0;
}
