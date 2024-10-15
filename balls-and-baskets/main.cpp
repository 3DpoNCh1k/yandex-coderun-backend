#include <algo/utils/io.hpp>
#include <algo/utils/types/fundamentals.hpp>
#include <algo/utils/types/modular.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;
using namespace algo::utils;

int main() {
  io::UnsyncIO();

  int n;
  cin >> n;
  vector<Modular_1_000_000_007> a(n);
  for (int i = 0; i < n; ++i) {
    i64 value;
    cin >> value;
    a[i] = value;
  }

  int Q;
  cin >> Q;
  int SQ = std::sqrt(n) + 1;

  vector<Modular_1_000_000_007> product(n / SQ + 1, 1);

  auto add_one = [&a](int l, int r) {
    for (int i = l; i <= r; ++i) {
      a[i] += 1;
    }
  };

  auto calculate_product_at = [&a, &product, SQ, n](int idx) {
    product[idx] = 1;
    for (int i = idx * SQ; i / SQ == idx && i < n; ++i) {
      product[idx] *= a[i];
    }
  };

  for (int idx = 0; idx <= n / SQ; ++idx) {
    calculate_product_at(idx);
  }

  for (int q = 0; q < Q; ++q) {
    int query_type, l, r;
    cin >> query_type >> l >> r;
    --l, --r;
    int left_idx = l / SQ;
    int right_idx = r / SQ;
    if (query_type == 0) {
      add_one(l, r);
      for (int m = left_idx; m <= right_idx; ++m) {
        calculate_product_at(m);
      }
    } else if (query_type == 1) {
      Modular_1_000_000_007 ans = 1;
      if (left_idx == right_idx) {
        for (int i = l; i <= r; ++i) {
          ans *= a[i];
        }
      } else {
        for (int m = left_idx + 1; m < right_idx; ++m) {
          ans *= product[m];
        }
        for (int i = l; i / SQ == left_idx && i < n; ++i) {
          ans *= a[i];
        }
        for (int i = r; i / SQ == right_idx && i >= 0; --i) {
          ans *= a[i];
        }
      }
      cout << ans.GetValue() << "\n";
    } else {
      std::abort();
    }
  }
}
