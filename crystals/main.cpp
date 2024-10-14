#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  string a, b, c;
  cin >> a >> b >> c;

  auto get_chunk = [](string s) {
    int k = 0;
    for (auto ch : s) {
      if (ch == s[0]) {
        k++;
      } else {
        break;
      }
    }
    return string(k, s[0]);
  };

  string answer;
  while (!a.empty() && !b.empty() && !c.empty()) {
    auto chunk_a = get_chunk(a);
    auto chunk_b = get_chunk(b);
    auto chunk_c = get_chunk(c);
    if (chunk_a[0] == chunk_b[0] && chunk_a[0] == chunk_c[0]) {
      auto ch = chunk_a[0];
      auto lengths = vector({chunk_a.size(), chunk_b.size(), chunk_c.size()});
      sort(lengths.begin(), lengths.end());
      auto k = lengths[1];
      answer += string(k, ch);

      a = a.substr(chunk_a.size());
      b = b.substr(chunk_b.size());
      c = c.substr(chunk_c.size());
    } else {
      answer = "IMPOSSIBLE";
      break;
    }
  }
  if (!a.empty() || !b.empty() || !c.empty()) {
    answer = "IMPOSSIBLE";
  }
  cout << answer << endl;
}