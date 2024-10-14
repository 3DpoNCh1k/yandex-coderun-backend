#include <array>
#include <deque>
#include <iostream>

#include <algo/utils/types/fundamentals.hpp>
#include <vector>

using namespace std;

int main() {
  std::array<int, 4> player;
  for (int i = 0; i < 4; ++i) {
    cin >> player[i];
  }
  int ans = 4;
  std::deque<vector<int>> q;
  q.push_back({});
  while (!q.empty()) {
    auto count = q.front();
    q.pop_front();

    if (count.size() == 4) {
      int s = 0;
      for (int i = 0; i < 4; ++i) {
        s += count[i];
      }
      if (s >= 2 && s <= 4) {
        int k = 0;
        for (int i = 0; i < 4; ++i) {
          k += count[i] != player[i];
        }
        ans = min(ans, k);
      }
    } else {
      for (int k = 0; k <= 4; ++k) {
        auto next = count;
        next.push_back(k);
        q.push_back(next);
      }
    }
  }

  cout << ans << endl;
}
