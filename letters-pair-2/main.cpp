
#include "algo/utils/io.hpp"
#include <algorithm>
#include <bitset>
#include <cassert>
#include <iostream>

#include <algo/data_structures/dsu.hpp>
#include <algo/maths/algebra/power.hpp>
#include <algo/utils/bits.hpp>
#include <algo/utils/types/fundamentals.hpp>
#include <algo/utils/types/modular.hpp>
#include <vector>

using namespace std;

using Modular = algo::utils::Modular_998_244_353;

int main() {
  algo::utils::io::UnsyncIO();

  i64 n, k;
  cin >> n >> k;
  vector<pair<int, int>> edges;
  const int A = 20;

  for (int i = 0; i < n; ++i) {
    string s;
    cin >> s;
    int v = s[0] - 'A';
    int u = s[1] - 'A';
    edges.emplace_back(v, u);
  }

  auto dsu = algo::data_structures::DSU(A);

  for (auto [v, u] : edges) {
    dsu.Merge(v, u);
  }

  bool connected = true;
  int p = -1;
  for (auto [v, u] : edges) {
    for (int w : {v, u}) {
      auto pp = dsu.Find(w);
      if (p != -1 && p != pp) {
        connected = false;
      }
      p = pp;
    }
  }

  Modular ans = 0;

  if (connected) {
    const auto N = 1 << A;
    vector<Modular> dp(N);
    dp[0] = 1;
    for (int i = 0; i < n; ++i) {
      auto [v, u] = edges[i];
      vector<Modular> next_dp(N);
      for (int mask = 0; mask < N; ++mask) {
        auto mask_bitset = bitset<A>(mask);
        int rem_v = mask_bitset[v];
        int rem_u = mask_bitset[u];

        for (int add = 1; add <= min(2, int(k)); ++add) {
          int next_rem_v = (rem_v + add) % 2;
          int next_rem_u = (rem_u + add) % 2;
          if (u == v) {
            next_rem_v = (rem_v + add + add) % 2;
            next_rem_u = next_rem_v;
          }

          auto next_mask_bitset = mask_bitset;
          next_mask_bitset.reset(v);
          next_mask_bitset.reset(u);
          if (next_rem_v) {
            next_mask_bitset.set(v);
          }
          if (next_rem_u) {
            next_mask_bitset.set(u);
          }

          auto next_mask = next_mask_bitset.to_ulong();

          auto can_increase = (k - add) / 2;
          Modular value = can_increase + 1;

          next_dp[next_mask] += dp[mask] * value;
        }
      }
      dp = next_dp;
    }

    for (int mask = 0; mask < N; ++mask) {
      int k_odd = algo::utils::bits::CountOfOnes(u64(mask));
      if (k_odd == 0 || k_odd == 2) {
        ans += dp[mask];
      }
    }
  }

  cout << ans.GetValue() << endl;
}
