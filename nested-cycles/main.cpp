#include <cassert>
#include <iostream>

#include <algo/utils/types/fundamentals.hpp>
#include <algo/utils/types/modular.hpp>
#include <vector>

using namespace std;

struct Bound {
  int ch = -1;
  int value = -1;
};

Bound ToBound(string s) {
  if (s.size() == 1 && s[0] >= 'a' && s[1] <= 'z') {
    return Bound{s[0] - 'a', -1};
  }
  return Bound{-1, stoi(s)};
}

const int mxN = 1e5 + 5;
using Modular = algo::utils::Modular_1_000_000_007;

struct Node {
  vector<Modular> dp;
  vector<Modular> cum_sum;
  vector<int> childs;
  int parent = -1;
  Bound LB;
  Bound RB;
  int index = -1;

  void Init() {
    dp.resize(mxN);
    cum_sum.resize(mxN);
    if (parent == -1) {
      auto l = LB.value;
      auto r = RB.value;
      assert(l != -1);
      assert(r != -1);
      for (int i = l; i <= r; ++i) {
        dp[i] = 1;
      }
    } else {
      for (int i = 0; i < mxN; ++i) {
        dp[i] = 1;
      }
    }
  }

  void CalculateCumSums() {
    cum_sum[0] = dp[0];
    for (int i = 1; i < mxN; ++i) {
      cum_sum[i] = cum_sum[i - 1] + dp[i];
    }
  }

  Modular GetFromRange(int l, int r) {
    if (l > r) {
      return 0;
    }
    auto sum_r = cum_sum[r];
    auto sum_l = l == 0 ? 0 : cum_sum[l - 1];
    return sum_r - sum_l;
  }

  auto GetCountWhenParentEquals(int value) {
    if (RB.value != -1) {
      auto r = RB.value;
      // [a..r]
      return GetFromRange(value, r);
    }
    auto l = LB.value;
    // [l..a]
    return GetFromRange(l, value);
  }
};

vector<Node> nodes;

Modular Calculate(int v) {
  Node &node = nodes[v];
  node.Init();
  for (int u : node.childs) {
    Calculate(u);
  }
  for (int u : node.childs) {
    Node &child_node = nodes[u];
    for (int i = 0; i < mxN; ++i) {
      node.dp[i] *= child_node.GetCountWhenParentEquals(i);
    }
  }
  node.CalculateCumSums();
  return node.GetFromRange(0, mxN - 1);
};

int main() {
  int n;
  cin >> n;
  nodes.resize(n);
  for (int i = 0; i < n; ++i) {
    string l, r;
    cin >> l >> r;

    Node &node = nodes[i];
    node.index = i;
    node.LB = ToBound(l);
    node.RB = ToBound(r);
    if (node.LB.ch != -1) {
      node.parent = node.LB.ch;
      nodes[node.parent].childs.push_back(i);
    }
    if (node.RB.ch != -1) {
      node.parent = node.RB.ch;
      nodes[node.parent].childs.push_back(i);
    }
    if (node.LB.value != -1) {
      assert(node.LB.value >= 1);
      assert(node.LB.value < mxN);
    }
    if (node.RB.value != -1) {
      assert(node.RB.value >= 1);
      assert(node.RB.value < mxN);
    }
  }

  Modular res = 1;
  for (int v = 0; v < n; ++v) {
    if (nodes[v].parent == -1) {
      res *= Calculate(v);
    }
  }
  cout << res.GetValue() << endl;
}
