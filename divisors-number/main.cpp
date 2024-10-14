#include <iostream>

#include <algo/utils/types/fundamentals.hpp>
#include <vector>

using namespace std;

int main() {
  int n;
  cin >> n;
  std::vector<int> count(n + 1, 1);
  for (i64 d = 2; d <= n; ++d) {
    for (i64 value = d; value <= n; value += d) {
      count[value]++;
    }
  }

  auto best = make_pair(0, 0);
  for (int i = 1; i <= n; ++i) {
    best = std::max(best, {count[i], i});
  }
  cout << best.second << endl;
  cout << best.first << endl;
}
