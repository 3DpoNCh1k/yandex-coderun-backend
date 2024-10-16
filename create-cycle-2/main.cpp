#include "algo/graphs/entity/edge.hpp"
#include "algo/graphs/entity/graph.hpp"
#include "algo/trees/entity/tree.hpp"
#include <cassert>
#include <deque>
#include <iostream>

#include <algo/graphs/bridges.hpp>
#include <algo/utils/types/fundamentals.hpp>
#include <algo/utils/types/modular.hpp>
#include <map>

using namespace std;

using namespace algo::graphs;
using namespace algo::trees;

using Modular = algo::utils::Modular_998_244_353;

vector<Tree> GetBridgeTrees(const UndirectedGraph &g) {
  int n = g.n;

  auto bridges = algo::graphs::FindBridges(g);
  auto g2 = algo::graphs::UndirectedGraph(n);
  for (auto e : bridges) {
    g2.AddEdge(e);
  }

  vector<bool> visited(n, false);
  vector<Tree> trees;

  for (int v = 0; v < n; ++v) {
    if (visited[v]) {
      continue;
    }
    visited[v] = true;

    deque<int> q = {v};
    vector<int> vertexes = {v};

    while (!q.empty()) {
      int v = q.front();
      q.pop_front();
      for (int e : g2.edge_list[v]) {
        int u = g2.edges[e].Neighbor(v);
        if (!visited[u]) {
          visited[u] = true;
          q.push_back(u);
          vertexes.push_back(u);
        }
      }
    }

    map<int, int> M;
    for (int i = 0; i < vertexes.size(); ++i) {
      M[vertexes[i]] = i;
    }
    Tree tree(vertexes.size());
    for (int v : vertexes) {
      int new_v = M[v];
      for (int e : g2.edge_list[v]) {
        int u = g2.edges[e].Neighbor(v);
        int new_u = M[u];
        if (new_v < new_u) {
          tree.AddEdge(new_v, new_u);
        }
      }
    }
    trees.push_back(tree);
  }

  return trees;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n, m;
  cin >> n >> m;

  UndirectedGraph g(n);
  for (int i = 0; i < m; ++i) {
    int v, u;
    cin >> v >> u;
    --v;
    --u;
    g.AddEdge(UndirectedEdge(v, u));
  }

  vector<Modular> F(n + 1);
  F[0] = {1};
  F[1] = {1};
  for (int num = 2; num <= n; ++num) {
    F[num] = F[num - 1] + (num - 1) * F[num - 2];
  }

  Modular ans = 1;
  auto trees = GetBridgeTrees(g);
  for (auto tree : trees) {
    for (int v = 0; v < tree.n; ++v) {
      ans *= F[tree.adjacency_list[v].size()];
    }
  }

  cout << ans.GetValue() << endl;
}
