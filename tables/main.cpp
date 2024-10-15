#include "algo/graphs/entity/edge.hpp"
#include "algo/graphs/entity/graph.hpp"
#include <cassert>
#include <iostream>

#include <algo/binary_search/binary_search.hpp>
#include <algo/graphs/scc.hpp>
#include <algo/utils/types/fundamentals.hpp>
#include <vector>

using namespace std;

int LongestIncreasingSubsequence(const vector<int> &a) {
  int n = a.size();
  if (n == 0) {
    return 0;
  }
  vector<int> dp(n + 1, 1e9);
  dp[0] = -1;
  for (int i = 0; i < n; ++i) {
    int best = algo::binary_search::FindLastTrue(
        0, n, [&](int len) { return dp[len] < a[i]; });
    dp[best + 1] = a[i];
  }
  int ans = 0;
  for (int len = n; len >= 0; --len) {
    if (dp[len] != 1e9) {
      ans = len;
      break;
    }
  }
  return ans;
};

int CalculateMoves(int n, int m, const vector<vector<int>> &values) {
  int ans = 0;
  for (int r = 0; r < n; ++r) {
    for (int v : values[r]) {
      if (v != r) {
        ans++;
      }
    }
  }

  auto g = algo::graphs::DirectedGraph(n);
  for (int r = 0; r < n; ++r) {
    for (int v : values[r]) {
      g.AddEdge(algo::graphs::DirectedEdge(r, v));
    }
  }

  auto [_, components] = algo::graphs::StronglyConnectedComponents(g);

  vector<int> S(n);

  for (auto &component : components) {
    bool ok = component.size() > 1;
    for (int v : component) {
      ok &= values[v].size() == m;
    }
    for (int v : component) {
      S[v] = 1;
    }
    for (int v : component) {
      for (int value : values[v]) {
        ok &= S[value] == 1;
      }
    }
    for (int v : component) {
      S[v] = 0;
    }
    if (ok) {
      ans++;
    }
  }

  return ans;
}

int main() {
  int n, m;
  cin >> n >> m;

  vector<vector<int>> t1(n, vector<int>(m));
  vector<vector<int>> t2(n, vector<int>(m));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      cin >> t1[i][j];
    }
  }

  bool has_empty = false;
  bool ok = true;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      cin >> t2[i][j];
      has_empty |= t2[i][j] == 0;
      ok &= t2[i][j] == t1[i][j];
    }
  }

  if (!has_empty) {
    cout << (ok ? 0 : -1) << endl;
    return 0;
  }

  vector<int> value_to_row(n * m + 1);
  vector<int> positions(n * m + 1);

  for (int r = 0; r < n; ++r) {
    int counter = 0;
    for (int c = 0; c < m; ++c) {
      int value = t2[r][c];
      if (value != 0) {
        value_to_row[value] = r;
        positions[value] = counter++;
      }
    }
  }

  vector<vector<int>> values(n);
  int perm_cost = 0;
  for (int r = 0; r < n; ++r) {
    vector<int> perm;
    for (int c = 0; c < m; ++c) {
      int value = t1[r][c];
      if (value != 0) {
        int row = value_to_row[value];
        values[r].push_back(row);

        if (row == r) {
          int pos = positions[value];
          perm.push_back(pos);
        }
      }
    }
    int lis = LongestIncreasingSubsequence(perm);
    perm_cost += perm.size() - lis;
    if (perm.size() == m && lis != m) {
      perm_cost++;
    }
  }

  int rows_cost = CalculateMoves(n, m, values);

  int ans = perm_cost + rows_cost;
  cout << ans << endl;
}
