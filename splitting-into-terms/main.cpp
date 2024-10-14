#include "algo/utils/join.hpp"
#include <algorithm>
#include <iostream>

#include <algo/utils/types/fundamentals.hpp>
#include <vector>

using namespace std;

std::vector<int> current_split;
std::vector<std::vector<int>> splits;
int current_sum;
int n;

void split(int limit) {
  if (current_sum == n) {
    splits.push_back(current_split);
    return;
  }
  for (int value = min(limit, n - current_sum); value >= 1; --value) {
    current_split.push_back(value);
    current_sum += value;
    split(value);
    current_split.pop_back();
    current_sum -= value;
  }
}

int main() {
  cin >> n;
  current_sum = 0;
  split(n);

  sort(splits.begin(), splits.end());

  for (auto split : splits) {
    cout << algo::utils::JoinToString(split, " + ") << endl;
  }
}
