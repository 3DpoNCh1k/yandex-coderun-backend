#include <iomanip>
#include <iostream>

#include <algo/utils/io.hpp>
#include <algo/utils/types/fundamentals.hpp>
#include <vector>

using namespace std;

bool Solve(const vector<int> &a0, int k, Real avg) {
  int n = a0.size();
  vector<Real> a(n);
  for (int i = 0; i < n; ++i) {
    a[i] = a0[i] - avg;
  }
  Real sum = 0;
  vector<Real> prefix_sum(n);
  Real min_prefix_sum = 0;
  for (int i = 0; i < n; ++i) {
    sum += a[i];
    prefix_sum[i] = sum;
    if (i >= k) {
      min_prefix_sum = min(min_prefix_sum, prefix_sum[i - k]);
    }
    if (i >= k - 1) {
      if (sum - min_prefix_sum >= 0) {
        return true;
      }
    }
  }

  return false;
}

int main() {
  algo::utils::io::UnsyncIO();

  int n, k;
  cin >> n >> k;
  vector<int> a(n);
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
  }

  Real lb = 0;
  Real rb = 1e6;
  Real eps = 1e-7;
  while (rb - lb > eps) {
    auto m = (lb + rb) / 2;
    if (Solve(a, k, m)) {
      lb = m;
    } else {
      rb = m;
    }
  }
  auto ans = lb;

  cout << setprecision(7) << fixed << ans << endl;
}
