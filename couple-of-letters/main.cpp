#include <algorithm>
#include <iostream>
#include <map>

using namespace std;

int main() {
  string word;
  map<string, int> counter;
  while (cin >> word) {
    for (int i = 0; i < int(word.size()) - 1; ++i) {
      counter[word.substr(i, 2)]++;
    }
  }

  auto [key, _] =
      *max_element(counter.begin(), counter.end(), [](auto kv1, auto kv2) {
        auto [key1, value1] = kv1;
        auto [key2, value2] = kv2;
        return value1 == value2 ? key1 < key2 : value1 < value2;
      });
  cout << key << endl;
}