#include <cassert>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

int main() {
  int R, C;
  cin >> R >> C;
  vector<int> heights(C + 1);
  pair<int, pair<int, int>> answer;
  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      int cell;
      cin >> cell;
      heights[c] = cell == 0 ? 0 : heights[c] + 1;
    }
    auto h = heights;
    int n = h.size();
    stack<int> st;
    for (int i = 0; i < n; ++i) {
      int idx = i;
      while (!st.empty() && h[st.top()] > h[i]) {
        int j = st.top();
        idx = j;
        st.pop();
        int value = min(h[j], i - j);
        if (h[j] > value && h[i] > value) {
          value++;
        }
        if (value > 0) {
          int length = value;
          int row_coordinate = r + 1 - (length - 1);
          int col_coordinate = j + 1;
          pair<int, pair<int, int>> current_answer = {
              length, {row_coordinate, col_coordinate}};
          answer = max(answer, current_answer);
        }
      }
      h[idx] = h[i];
      st.push(idx);
    }
  }
  auto [length, coordinate] = answer;
  auto [row_coordinate, col_coordinate] = coordinate;
  cout << length << endl;
  cout << row_coordinate << " " << col_coordinate << endl;
}
