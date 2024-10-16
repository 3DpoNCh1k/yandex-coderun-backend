#include "algo/utils/io.hpp"
#include <cassert>
#include <iostream>

#include <algo/ranges/convex_hull_tree.hpp>
#include <algo/utils/bits.hpp>
#include <algo/utils/types/128_t.hpp>
#include <algo/utils/types/fundamentals.hpp>
#include <vector>

using namespace std;

int main() {
  algo::utils::io::UnsyncIO();

  int n, m;
  cin >> n >> m;

  vector<i64> s(n);
  vector<i64> p(n);
  for (int i = 0; i < n; ++i) {
    cin >> s[i] >> p[i];
  }

  using Tree = algo::ranges::MinimumConvexHullTree<algo::utils::int128_t>;
  using Line = Tree::CHT::Line;

  vector<Line> lines(n);
  for (int i = 0; i < n; ++i) {
    auto b = p[i] + s[i] * s[i];
    auto k = -2 * s[i];
    lines[i] = Line(k, b);
  }

  Tree tree(lines);

  for (int i = 0; i < m; ++i) {
    i64 c, k;
    cin >> c >> k;
    vector<int> d(k);

    for (int j = 0; j < k; ++j) {
      cin >> d[j];
      d[j]--;
    }

    i64 ans = 2e18;

    d.push_back(n);
    sort(d.begin(), d.end());
    int last = -1;
    for (int forbidden : d) {
      int l = last + 1;
      int r = forbidden - 1;
      if (l <= r) {
        ans = min(ans, i64(tree.Get(l, r, c)));
      }
      last = forbidden;
    }

    ans += c * c;

    cout << ans << endl;
  }
}
