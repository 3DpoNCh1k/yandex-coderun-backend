#include "algo/utils/io.hpp"
#include <algorithm>
#include <iostream>

#include <algo/maths/algebra/factorization/factorize.hpp>
#include <algo/utils/types/fundamentals.hpp>
#include <map>
#include <numeric>
#include <vector>

using namespace std;

struct Query {
  int q_index;
  int x_index;
  int d;
  int cnt;
};

void ParallelBinarySearch(const vector<Query> &queries,
                          const vector<vector<i32>> &fact,
                          vector<vector<int>> &best_xs) {
  int n = fact.size();
  int q = queries.size();
  vector<int> L(q, 0);
  vector<int> R(q, n - 1);

  bool stop = false;
  while (!stop) {
    stop = true;
    vector<vector<int>> query_map(n);
    for (int i = 0; i < q; ++i) {
      int m = (L[i] + R[i] + 1) / 2;
      query_map[m].push_back(i);
    }

    map<int, int> FM;
    for (int i = 1; i < n; ++i) {
      for (int d : fact[i]) {
        FM[d]++;
      }
      for (int qi : query_map[i]) {
        auto query = queries[qi];
        if (FM[query.d] < query.cnt) {
          L[qi] = i;
        } else {
          R[qi] = i - 1;
        }
      }
    }

    for (int i = 0; i < q; ++i) {
      if (L[i] != R[i]) {
        stop = false;
      }
    }
  }

  for (int i = 0; i < q; ++i) {
    auto query = queries[i];
    best_xs[query.q_index][query.x_index] =
        max(best_xs[query.q_index][query.x_index], L[i]);
  }
};

int main() {
  algo::utils::io::UnsyncIO();

  int n, m, q;
  cin >> n >> m >> q;
  vector<int> a(n + 1), b(m + 1), ca(n + 1), cb(m + 1);
  for (int i = 0; i < n + 1; ++i) {
    cin >> a[i];
  }
  for (int i = 0; i < m + 1; ++i) {
    cin >> b[i];
  }
  for (int i = 0; i < n + 1; ++i) {
    cin >> ca[i];
  }
  for (int i = 0; i < m + 1; ++i) {
    cin >> cb[i];
  }

  for (int i = 1; i < n + 1; ++i) {
    ca[i] = min(ca[i], ca[i - 1]);
  }
  for (int i = 1; i < m + 1; ++i) {
    cb[i] = min(cb[i], cb[i - 1]);
  }

  vector<vector<i32>> fact_a(n + 1), fact_b(m + 1);
  for (int i = 0; i < n + 1; ++i) {
    fact_a[i] = algo::maths::algebra::factorization::Factorize(a[i]);
  }

  for (int i = 0; i < m + 1; ++i) {
    fact_b[i] = algo::maths::algebra::factorization::Factorize(b[i]);
  }

  vector<Query> queries;

  vector<vector<int>> best_as(q);
  vector<vector<int>> best_bs(q);

  for (int i = 0; i < q; ++i) {
    int k;
    cin >> k;

    best_as[i].resize(k, -1);
    best_bs[i].resize(k, -1);

    for (int j = 0; j < k; ++j) {
      int x;
      cin >> x;
      map<int, int> m_x;
      for (int d : algo::maths::algebra::factorization::Factorize(x)) {
        m_x[d]++;
      }
      for (auto [d, cnt] : m_x) {
        queries.push_back({i, j, d, cnt});
      }
    }
  }

  ParallelBinarySearch(queries, fact_a, best_as);
  ParallelBinarySearch(queries, fact_b, best_bs);

  for (int qi = 0; qi < q; ++qi) {
    int k = best_bs[qi].size();
    auto &best_b = best_bs[qi];
    auto &best_a = best_as[qi];
    vector<int> order(k);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(),
         [&](int i, int j) { return best_b[i] < best_b[j]; });

    int min_a = n;
    int ans = 2e9;

    for (int i : order) {
      auto b_i = best_b[i];
      auto a_i = min_a;
      auto cur_ans = ca[a_i] + cb[b_i];
      ans = min(ans, cur_ans);
      min_a = min(min_a, best_a[i]);
    }
    auto cur_ans = ca[min_a] + cb.back();
    ans = min(ans, cur_ans);
    cout << ans << "\n";
  }
}
