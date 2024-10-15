#include <iostream>

#include <algo/utils/types/fundamentals.hpp>
#include <utility>
#include <vector>

using namespace std;

int main() {
  int n;
  cin >> n;
  vector<vector<int>> g(n, vector<int>(n));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      cin >> g[i][j];
    }
  }

  const int inf = 1e9;

  auto dijkstra = [&](std::pair<int, int> forbidden) {
    vector<pair<int, int>> d(n, {inf, -1});
    vector<bool> visited(n, false);
    d[0] = {0, -1};
    while (true) {
      int v = -1;
      for (int u = 0; u < n; ++u) {
        if (!visited[u] && d[u].first != inf) {
          if (v == -1 || d[v] > d[u]) {
            v = u;
          }
        }
      }
      if (v == -1) {
        break;
      }
      visited[v] = true;
      for (int u = 0; u < n; ++u) {
        if (g[v][u] != -1 && !visited[u]) {
          if (make_pair(u, v) != forbidden && make_pair(v, u) != forbidden) {
            d[u] = min(d[u], {d[v].first + g[v][u], v});
          }
        }
      }
    }
    return d;
  };

  auto d = dijkstra({-1, -1});
  int ans = 0;
  for (int v = 0; v < n; ++v) {
    int u = d[v].second;
    auto d2 = dijkstra({u, v});
    int cur = 0;
    for (int i = 0; i < n; ++i) {
      cur += d[i].first != d2[i].first;
    }
    ans = max(ans, cur);
  }

  cout << ans << endl;
}
