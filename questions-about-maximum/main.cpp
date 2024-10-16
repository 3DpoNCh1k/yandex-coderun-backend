#include "algo/utils/join.hpp"
#include <algorithm>
#include <array>
#include <iostream>

#include <algo/utils/types/fundamentals.hpp>
#include <vector>

using namespace std;

int main() {
  int n, q;
  cin >> n >> q;
  vector<array<int, 3>> queries(q);
  vector<array<int, 3>> events;
  for (int i = 0; i < q; ++i) {
    int l, r, x;
    cin >> l >> r >> x;
    --l;
    --r;
    queries[i] = {l, r, x};

    events.push_back({l, 0, i});
    events.push_back({r, 1, i});
  }

  sort(events.begin(), events.end());

  vector<int> backpack(n + 1);
  backpack[0] = 1;

  auto add = [&backpack](int value) {
    int B = backpack.size();
    for (int i = B - 1; i >= value; --i) {
      backpack[i] += backpack[i - value];
    }
  };

  auto remove = [&backpack](int value) {
    int B = backpack.size();
    for (int i = value; i < B; ++i) {
      backpack[i] -= backpack[i - value];
    }
  };

  vector<int> can(n + 1);
  for (auto &e : events) {
    auto [pos, type, idx] = e;
    auto value = queries[idx][2];
    if (type == 0) {
      add(value);
      for (int i = 1; i <= n; ++i) {
        if (backpack[i] > 0) {
          can[i] = 1;
        }
      }
    } else {
      remove(value);
    }
  }

  vector<int> answer;
  for (int i = 1; i <= n; ++i) {
    if (can[i] == 1) {
      answer.push_back(i);
    }
  }
  cout << answer.size() << endl;
  cout << algo::utils::JoinToString(answer, " ") << endl;
}
