#include <cassert>
#include <deque>
#include <iostream>

#include <algo/graphs/bridges.hpp>
#include <algo/graphs/entity/graph.hpp>
#include <algo/utils/types/fundamentals.hpp>
#include <vector>

using namespace std;

int main() {
  int n, m;
  cin >> n >> m;
  auto g = algo::graphs::UndirectedGraph(n);

  for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    --u;
    --v;
    g.AddEdge(algo::graphs::UndirectedEdge(u, v));
  }

  auto bridges = algo::graphs::FindBridges(g);
  auto g2 = algo::graphs::UndirectedGraph(n);
  for (auto e : bridges) {
    g2.AddEdge(e);
  }
  vector<bool> visited(n, false);
  i64 answer = 0;
  for (int v = 0; v < n; ++v) {
    if (visited[v]) {
      continue;
    }
    visited[v] = true;
    i64 cnt = 1;
    deque<int> q = {v};
    vector<int> vertexes = {v};

    while (!q.empty()) {
      int v = q.front();
      q.pop_front();
      for (int e : g2.edge_list[v]) {
        int u = g2.edges[e].Neighbor(v);
        if (!visited[u]) {
          visited[u] = true;
          cnt++;
          q.push_back(u);
          vertexes.push_back(u);
        }
      }
    }

    i64 s = 0;
    for (int v : vertexes) {
      s += (cnt - 1 - g2.edge_list[v].size());
    }

    answer += s / 2;
  }

  cout << answer << endl;
}
