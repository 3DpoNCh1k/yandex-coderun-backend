#include <algorithm>
#include <cassert>
#include <iostream>

#include <algo/utils/types/fundamentals.hpp>
#include <set>
#include <vector>

using namespace std;

int main() {
  int n;
  cin >> n;
  vector<pair<int, int>> tasks(n);
  for (int i = 0; i < n; ++i) {
    int d, w;
    cin >> d >> w;
    tasks[i] = {d, w};
  }
  sort(tasks.begin(), tasks.end());
  multiset<pair<int, int>> current_tasks;
  i64 ans = 0;
  for (auto [d, w] : tasks) {
    current_tasks.insert({w, d});
    assert(current_tasks.size() <= d + 1);
    if (current_tasks.size() == d + 1) {
      auto it = current_tasks.begin();
      ans += it->first;
      current_tasks.erase(it);
    }
  }
  cout << ans << endl;
}
