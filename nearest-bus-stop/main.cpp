#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;
int main() {
  int n, k;
  cin >> n >> k;
  vector<int> stop(n);
  for (int &pos : stop) {
    cin >> pos;
  }
  for (int i = 0; i < k; ++i) {
    int bus;
    cin >> bus;
    auto it = lower_bound(stop.begin(), stop.end(), bus);
    int answer = n;
    if (it != stop.end()) {
      int stop_idx = distance(stop.begin(), it);
      answer = stop_idx + 1;
      if (stop_idx - 1 >= 0 &&
          (bus - stop[stop_idx - 1] <= stop[stop_idx] - bus)) {
        answer--;
      }
    }
    cout << answer << "\n";
  }
}