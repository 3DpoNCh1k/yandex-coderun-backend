#include <cassert>
#include <iostream>

#include <algo/utils/types/fundamentals.hpp>
#include <map>
#include <vector>

using namespace std;

int main() {
  string s;
  cin >> s;
  int n = s.size();
  vector<vector<int>> table(26, vector<int>(n + 1));
  for (int i = 0; i < n; ++i) {
    table[s[i] - 'a'][i + 1] = 1;
  }

  for (int j = 0; j < 26; ++j) {
    for (int i = 1; i <= n; ++i) {
      table[j][i] += table[j][i - 1];
    }
  }

  auto get = [&](int ch, int l, int r) {
    return table[ch][r + 1] - table[ch][l];
  };

  auto ok = [&](int i, int len) {
    bool ok = true;
    for (int ch = 0; ok && ch < 26; ++ch) {
      ok &= get(ch, 0, len - 1) == get(ch, i, i + len - 1);
    }
    return ok;
  };

  vector<int> divisors;
  for (int d = 1; d <= n; ++d) {
    if (n % d == 0) {
      divisors.push_back(d);
    }
  }

  map<int, int> min_k;
  int best = -1;
  for (int d : divisors) {
    int k = min_k[d];
    int max_k = n / d;
    while (k < max_k) {
      int i = k * d;
      if (!ok(i, d)) {
        break;
      }
      k++;
    }
    if (k == max_k) {
      best = d;
      break;
    }
    for (int m : divisors) {
      if (d * m <= n) {
        int cur_min_k = k / m;
        min_k[d * m] = max(min_k[d * m], cur_min_k);
      }
    }
  }

  assert(best != -1);

  cout << n / best << endl;
}
