#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>

#include <algo/maths/algebra/gcd.hpp>
#include <algo/utils/types/fundamentals.hpp>
#include <set>

using namespace std;

i64 CountFromOneToN(i64 n, i64 divisor) {
  assert(n >= 0);
  return n / divisor;
}

i64 Solve(i64 n, vector<i64> coefs) {
  auto mod = coefs.back();
  coefs.pop_back();
  vector<i64> dist(mod, -1);
  set<i64> q;
  dist[0] = 0;
  q.insert(0);
  i64 answer = 0;
  while (!q.empty()) {
    auto it = q.begin();
    auto v = *it;
    q.erase(it);
    if (v >= n) {
      continue;
    }
    auto d = n - 1 - v;
    auto cnt = 1 + CountFromOneToN(d, mod);
    answer += cnt;
    for (auto coef : coefs) {
      auto next_v = v + coef;
      if (dist[next_v % mod] == -1 || dist[next_v % mod] > next_v) {
        q.erase(dist[next_v % mod]);
        q.insert(next_v);
        dist[next_v % mod] = next_v;
      }
    }
  }
  return answer;
}

int main() {
  i64 n;
  cin >> n;
  array<i64, 3> coefs;
  cin >> coefs[0] >> coefs[1] >> coefs[2];
  sort(coefs.begin(), coefs.end());
  auto [a, b, c] = coefs;
  i64 answer = Solve(n, {a, b, c});
  cout << answer << endl;
}
