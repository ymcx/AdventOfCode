#include "misc/io.h"
#include "misc/point.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <map>
#include <string>
#include <vector>

using namespace boost;
using namespace std;
using Graph = adjacency_list<listS, vecS, directedS, no_property,
                             property<edge_weight_t, int>>;
using Vertex = graph_traits<Graph>::vertex_descriptor;

struct State {
  DirectionalPoint point;
  int count;

  bool operator<(const State &other) const {
    if (point.y != other.point.y) {
      return point.y < other.point.y;
    }
    if (point.x != other.point.x) {
      return point.x < other.point.x;
    }
    if (point.direction != other.point.direction) {
      return point.direction < other.point.direction;
    }
    return count < other.count;
  }
};

pair<vector<vector<int>>, Point> parse_grid(string path) {
  vector<string> lines = read_lines(path);
  Point dimensions = Point(lines.size(), lines[0].size());
  vector<vector<int>> grid(dimensions.y, vector<int>(dimensions.x));

  for (int y = 0; y < dimensions.y; ++y) {
    for (int x = 0; x < dimensions.x; ++x) {
      grid[y][x] = lines[y][x] - '0';
    }
  }

  return {grid, dimensions};
}

int dijkstra(const vector<vector<int>> &grid, Point dimensions, int min_step,
             int max_step) {
  Graph graph;
  map<State, Vertex> state_to_vertex;

  auto get_vertex = [&](State state) {
    if (state_to_vertex.contains(state)) {
      return state_to_vertex[state];
    }
    Vertex vertex = add_vertex(graph);
    state_to_vertex[state] = vertex;
    return vertex;
  };

  for (int y = 0; y < dimensions.y; ++y) {
    for (int x = 0; x < dimensions.x; ++x) {
      for (int direction = RIGHT; direction <= UP; ++direction) {
        DirectionalPoint point = DirectionalPoint(y, x, direction);

        for (int count = 1; count <= max_step; ++count) {
          State state_from = State(point, count);
          Vertex vertex_from = get_vertex(state_from);

          for (int direction_next = RIGHT; direction_next <= UP;
               ++direction_next) {
            DirectionalPoint point_next = point.move(direction_next);

            if (!point_next.is_valid(dimensions) ||
                (direction + 2) % 4 == direction_next) {
              continue;
            }

            int count_next;
            if (direction == direction_next) {
              count_next = count + 1;
              if (count_next > max_step) {
                continue;
              }
            } else {
              count_next = 1;
              if (count < min_step) {
                continue;
              }
            }

            State state_to = State(point_next, count_next);
            Vertex vertex_to = get_vertex(state_to);
            int cost = grid[point_next.y][point_next.x];
            add_edge(vertex_from, vertex_to, cost, graph);
          }
        }
      }
    }
  }

  vector<int> distances(num_vertices(graph), numeric_limits<int>::max());
  for (int direction = RIGHT; direction <= UP; ++direction) {
    DirectionalPoint point = DirectionalPoint(0, 0, direction);
    State state = State(point, 1);
    Vertex vertex = get_vertex(state);
    vector<int> temp(num_vertices(graph), numeric_limits<int>::max());

    dijkstra_shortest_paths(graph, vertex, distance_map(&temp[0]));

    for (int i = 0; i < distances.size(); ++i) {
      distances[i] = min(distances[i], temp[i]);
    }
  }

  int cost = numeric_limits<int>::max();
  for (int direction = RIGHT; direction <= UP; ++direction) {
    DirectionalPoint point =
        DirectionalPoint(dimensions.y - 1, dimensions.x - 1, direction);
    for (int count = min_step; count <= max_step; ++count) {
      State state = State(point, count);

      if (state_to_vertex.contains(state)) {
        Vertex vertex = state_to_vertex[state];
        cost = min(cost, distances[vertex]);
      }
    }
  }

  return cost;
}

int main(int argc, char *argv[]) {
  auto [grid, dimensions] = parse_grid(argv[1]);

  int p1 = dijkstra(grid, dimensions, 1, 3);
  int p2 = dijkstra(grid, dimensions, 4, 10);

  assert_print(p1, p2, 1155, 1283);
}
