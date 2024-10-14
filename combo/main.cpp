#include <array>
#include <iostream>

#include <algo/utils/types/fundamentals.hpp>
#include <vector>

using namespace std;

int main() {
  int n;
  cin >> n;
  vector<int> price(n);
  for (int i = 0; i < n; ++i) {
    cin >> price[i];
  }

  int X;
  cin >> X;
  array<int, 4> combo;
  for (int i = 0; i < 4; ++i) {
    cin >> combo[i];
    combo[i]--;
  }

  int k;
  cin >> k;
  vector<int> count(n);
  for (int i = 0; i < k; ++i) {
    int j;
    cin >> j;
    j--;
    count[j]++;
  }

  int ans = 1e6;
  for (int k_combo = 0; k_combo <= k; ++k_combo) {
    int s = 0;
    vector<int> cart(n);
    for (int i = 0; i < k_combo; ++i) {
      for (auto j : combo) {
        cart[j]++;
      }
      s += X;
    }
    for (int i = 0; i < n; ++i) {
      if (cart[i] < count[i]) {
        s += price[i] * (count[i] - cart[i]);
      }
    }
    ans = min(ans, s);
  }

  cout << ans << endl;
}
