
#include <algorithm>
#include <cassert>
#include <iostream>

#include <algo/maths/geometry/convex_hull.hpp>
#include <algo/utils/types/fundamentals.hpp>
#include <vector>

using namespace std;

using Point = algo::maths::geometry::Point<i64>;

bool Solve(const vector<Point> &a) {
  int n = a.size();
  assert(n <= 6);
  if (n < 4) {
    return false;
  }
  vector<int> perm;
  for (int i = 0; i < n; ++i) {
    perm.push_back(i);
  }
  bool ok = false;
  do {
    vector<Point> points;
    for (int i = 0; i < 4; ++i) {
      points.push_back(a[perm[i]]);
    }
    ok |= algo::maths::geometry::ConvexHull(points).size() == 4;
  } while (next_permutation(perm.begin(), perm.end()));

  return ok;
}

int main() {
  int n;
  cin >> n;

  vector<Point> a(n, Point(0, 0));
  for (int i = 0; i < n; ++i) {
    cin >> a[i].x >> a[i].y;
  }

  auto points = algo::maths::geometry::ConvexHull(a);

  if (points.size() >= 4) {
    cout << "Yes" << endl;
    return 0;
  }

  assert(points.size() <= 3);

  if (points.size() < 3) {
    cout << "No" << endl;
    return 0;
  }

  vector<Point> a2;
  for (auto p : a) {
    if (find(points.begin(), points.end(), p) == points.end()) {
      a2.push_back(p);
    }
  }

  auto points2 = algo::maths::geometry::ConvexHull(a2);

  if (points2.size() >= 4) {
    cout << "Yes" << endl;
    return 0;
  }

  assert(points2.size() <= 3);

  for (auto p2 : points2) {
    points.push_back(p2);
  }

  cout << (Solve(points) ? "Yes" : "No") << endl;
}
