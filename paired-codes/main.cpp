#include <iostream>
#include <vector>

#include <algo/utils/types/fundamentals.hpp>

using namespace std;

int main() {
  int n;
  cin >> n;
  int C = 1 << 10;
  vector<int> counter(C);
  i64 answer = 0;
  for (int i = 0; i < n; ++i) {
    int a;
    cin >> a;
    int mask = 0;
    for (auto ch : to_string(a)) {
      int digit = ch - '0';
      mask |= 1 << digit;
    }
    int inversed_mask = (C - 1) ^ mask;
    int s = 0;
    for (int submask = inversed_mask; submask > 0;
         submask = (submask - 1) & inversed_mask) {
      s += counter[submask];
    }
    answer += i - s;
    counter[mask]++;
  }
  cout << answer << endl;
}
