#include "algo/utils/io.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>

#include <algo/binary_search/binary_search.hpp>
#include <algo/data_structures/dsu.hpp>
#include <algo/utils/join.hpp>
#include <algo/utils/types/fundamentals.hpp>

#include <utility>
#include <vector>

using namespace std;

struct Circle {
  i64 x;
  i64 y;
  i64 radius;
};

struct Query {
  i64 radius;
  i64 start;

  bool operator<(const Query &that) const { return radius < that.radius; }
};

bool canPassBetween(const Circle &circle0, const Circle &circle1, i64 radius) {
  auto dx2 = circle0.x - circle1.x;
  dx2 *= dx2;
  auto dy2 = circle0.y - circle1.y;
  dy2 *= dy2;
  auto dist = 2 * radius + circle0.radius + circle1.radius;
  if (dist >= 3e9) {
    return false;
  }
  return dx2 + dy2 >= dist * dist;
}

bool canPassBetween(const Circle &circle, int wall, i64 radius, i64 w, i64 h) {
  if (wall == 0) {
    return circle.y - circle.radius >= 2 * radius;
  }
  if (wall == 1) {
    return circle.x + circle.radius <= w - 2 * radius;
  }
  if (wall == 2) {
    return circle.y + circle.radius <= h - 2 * radius;
  }
  return circle.x - circle.radius >= 2 * radius;
}

vector<int> Solve(i64 w, i64 h, const vector<Circle> &circles, i64 radius,
                  int start) {
  int n = circles.size();
  auto dsu = algo::data_structures::DSU(n + 4);
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      if (!canPassBetween(circles[i], circles[j], radius)) {
        dsu.Merge(i, j);
      }
    }
  }

  for (int i = 0; i < n; ++i) {
    for (int wall = 0; wall < 4; ++wall) {
      if (!canPassBetween(circles[i], wall, radius, w, h)) {
        dsu.Merge(i, wall + n);
      }
    }
  }

  array<bool, 4> is_blocked_corner = {};

  for (int i = 0; i < 4; ++i) {
    int wall_0 = i;
    int wall_1 = i == 0 ? 3 : i - 1;
    if (dsu.Find(wall_0 + n) == dsu.Find(wall_1 + n)) {
      is_blocked_corner[i] = true;
    }
  }

  bool vertical_line = dsu.Find(0 + n) == dsu.Find(2 + n);
  bool horizontal_line = dsu.Find(1 + n) == dsu.Find(3 + n);

  array<int, 4> horizontal = {3, 2, 1, 0};
  array<int, 4> vertical = {1, 0, 3, 2};
  array<int, 4> diagonal = {2, 3, 0, 1};

  start--;
  vector<int> corners;
  corners.push_back(start);
  if (!is_blocked_corner[start]) {
    if (!is_blocked_corner[horizontal[start]] && !horizontal_line) {
      corners.push_back(horizontal[start]);
    }
    if (!is_blocked_corner[vertical[start]] && !vertical_line) {
      corners.push_back(vertical[start]);
    }
    if (!is_blocked_corner[diagonal[start]] && !horizontal_line &&
        !vertical_line) {
      corners.push_back(diagonal[start]);
    }
  }
  sort(corners.begin(), corners.end());
  for (auto &c : corners) {
    c++;
  }
  return corners;
};

int main() {
  algo::utils::io::UnsyncIO();

  int n, m;
  cin >> n >> m;
  i64 w, h;
  cin >> w >> h;
  vector<Circle> circles(n);
  for (int i = 0; i < n; ++i) {
    cin >> circles[i].x;
    cin >> circles[i].y;
    cin >> circles[i].radius;
  }

  vector<pair<Query, int>> queries(m);

  for (int i = 0; i < m; ++i) {
    auto q = Query();
    cin >> q.radius;
    cin >> q.start;
    queries[i] = {q, i};
  }

  for (auto [q, _] : queries) {
    auto corners = Solve(w, h, circles, q.radius, q.start);
    cout << algo::utils::JoinToString(corners, "") << endl;
  }
}
