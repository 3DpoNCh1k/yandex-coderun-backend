#include <iostream>

#include <algo/utils/types/fundamentals.hpp>
#include <map>

using namespace std;

int main() {
  int n;
  cin >> n;
  std::map<string, i64> score;
  for (int i = 0; i < n; ++i) {
    string name;
    cin >> name;
    score[name] = 0;
  }
  i64 a = 0, b = 0;
  int m;
  cin >> m;
  for (int i = 0; i < m; ++i) {
    i64 a_now, b_now;
    char sep;
    string name;
    cin >> a_now >> sep >> b_now >> name;
    i64 s = a_now - a + b_now - b;
    score[name] += s;
    a = a_now;
    b = b_now;
  }
  auto ans = score.begin()->first;
  for (auto [name, s] : score) {
    if (s > score[ans] || (s == score[ans] && name > ans)) {
      ans = name;
    }
  }
  cout << ans << " " << score[ans] << endl;
}
