#include <array>
#include <cassert>
#include <iostream>

#include <algo/maths/algebra/gcd.hpp>
#include <algo/utils/types/fundamentals.hpp>

using namespace std;

int main() {
  array<int, 2> start;
  array<int, 2> duration;

  auto to_min = [](int h, int m) { return h * 60 + m; };
  for (int i = 0; i < 2; ++i) {
    int h, m;
    char sep;
    cin >> h >> sep >> m;
    start[i] = to_min(h, m);
  }

  for (int i = 0; i < 2; ++i) {
    int h, m;
    char sep;
    cin >> h >> sep >> m;
    duration[i] = to_min(h, m);
  }

  if (start[0] > start[1]) {
    swap(start[0], start[1]);
    swap(duration[0], duration[1]);
  }

  int c = start[1] - start[0];
  assert(c > 0);

  i64 a = duration[0];
  i64 b = duration[1];
  i64 x, y;
  auto g = algo::maths::algebra::ExtendedGCD(a, b, x, y);

  if (c % g != 0) {
    cout << "Never";
    return 0;
  }

  auto k0 = x * (c / g);
  auto k1 = -y * (c / g);

  if (k0 <= 0) {
    auto step = duration[1] / g;
    auto d = -k0 + 1;
    k0 += ((d + step - 1) / step) * step;
  } else {
    auto step = duration[1] / g;
    auto d = k0 - 1;
    k0 -= (d / step) * step;
  }
  assert(k0 > 0);

  k1 = (k0 * duration[0] - c) / duration[1];
  if (k1 <= 0) {
    auto step = duration[0] / g;
    auto d = -k1 + 1;
    k1 += ((d + step - 1) / step) * step;
  }
  k0 = (c + k1 * duration[1]) / duration[0];

  assert(k0 > 0 && k1 > 0);

  auto t = start[0] + duration[0] * k0;
  auto MOD = to_min(24, 0);
  auto day = t / MOD;
  int min = t % MOD;

  int d = day % 7;
  auto h = min / 60;
  auto m = min % 60;
  array<string, 7> DAYS = {"Saturday",  "Sunday",   "Monday", "Tuesday",
                           "Wednesday", "Thursday", "Friday"};

  cout << DAYS[d] << "\n";
  printf("%02d:%02d", h, m);
}
