#include <iostream>
#include <map>

#include <algo/utils/types/fundamentals.hpp>
#include <string>

using namespace std;

auto GetDivisors(int value) {
  map<int, int> counter;
  for (int d = 2; d * d <= value; ++d) {
    int k = 0;
    while (value % d == 0) {
      ++k;
      value /= d;
    }
    if (k > 0) {
      counter[d] += k;
    }
  }
  if (value > 1) {
    counter[value]++;
  }
  return counter;
};

int main() {
  int n;
  cin >> n;
  map<int, int> counter_a;
  for (int i = 0; i < n; ++i) {
    int a;
    cin >> a;
    for (auto [d, cnt] : GetDivisors(a)) {
      counter_a[d] += cnt;
    }
  }
  int m;
  cin >> m;
  map<int, int> counter_b;
  for (int i = 0; i < m; ++i) {
    int b;
    cin >> b;
    for (auto [d, cnt] : GetDivisors(b)) {
      counter_b[d] += cnt;
    }
  }

  i64 MOD = 1e9;
  bool is_bigger = 0;
  i64 value = 1;
  for (auto [d, cnt] : counter_a) {
    auto k = min(cnt, counter_b[d]);
    for (int i = 0; i < k; ++i) {
      value *= d;
      if (value >= MOD) {
        is_bigger = 1;
      }
      value %= MOD;
    }
  }

  string ans = to_string(value);
  if (is_bigger) {
    ans = string(9 - ans.size(), '0') + ans;
  }
  cout << ans << endl;
}
