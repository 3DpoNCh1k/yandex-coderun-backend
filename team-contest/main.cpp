#include <algorithm>
#include <array>
#include <iostream>

#include <algo/ranges/prefix_sums.hpp>
#include <algo/utils/types/fundamentals.hpp>
#include <vector>

using namespace std;

int Solve(const array<vector<int>, 3> &a, int i, int j, int k) {
  int n = a[0].size();
  vector<int> dp(n, 1e9);
  auto sums = algo::ranges::PrefixSums(a[j]);
  dp[1] = a[j][0] + a[k][1];
  for (int index = 2; index < n; ++index) {
    dp[index] =
        min(dp[index - 1], (int)sums.GetFromPrefix(index - 1)) + a[k][index];
  }
  int ans = 1e9;
  int s = 0;
  for (int index = n - 1; index >= 2; --index) {
    s += a[i][index];
    ans = min(ans, s + dp[index - 1]);
  }
  return ans;
};

int main() {
  int n;
  cin >> n;
  array<vector<int>, 3> a;
  for (int i = 0; i < 3; ++i) {
    a[i].resize(n);
    for (int j = 0; j < n; ++j) {
      cin >> a[i][j];
    }
  }
  int ans = 1e9;
  vector<int> order = {0, 1, 2};

  do {
    ans = min(ans, Solve(a, order[0], order[1], order[2]));
  } while (next_permutation(order.begin(), order.end()));

  cout << ans << endl;
}
