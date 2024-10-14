#include <iostream>

#include <algo/utils/types/fundamentals.hpp>
#include <string>
#include <vector>

using namespace std;

int main() {
  int n, m;
  cin >> n >> m;
  vector<string> grid(n);
  for (int i = 0; i < n; ++i) {
    cin >> grid[i];
  }
  bool ans = false;
  for (int r = 0; r < n; ++r) {
    for (int c = 0; c < m; ++c) {
      if (grid[r][c] == '.') {
        continue;
      }
      if (r + 4 < n) {
        bool ok = true;
        for (int i = 0; i <= 4; ++i) {
          ok &= grid[r + i][c] == grid[r][c];
        }
        ans |= ok;
      }
      if (c + 4 < m) {
        bool ok = true;
        for (int i = 0; i <= 4; ++i) {
          ok &= grid[r][c + i] == grid[r][c];
        }
        ans |= ok;
      }
      if (r + 4 < n && c + 4 < m) {
        bool ok = true;
        for (int i = 0; i <= 4; ++i) {
          ok &= grid[r + i][c + i] == grid[r][c];
        }
        ans |= ok;
      }
      if (r - 4 >= 0 && c + 4 < m) {
        bool ok = true;
        for (int i = 0; i <= 4; ++i) {
          ok &= grid[r - i][c + i] == grid[r][c];
        }
        ans |= ok;
      }
    }
  }
  cout << (ans ? "Yes" : "No") << endl;
}
