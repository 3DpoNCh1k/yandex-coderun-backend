#include <iostream>

#include <algo/utils/types/fundamentals.hpp>

using namespace std;

int main() {
  /*
   2n + 2(m-2) == B
   2(n+m) - 4 == B
   2(n+m) == B + 4
   n + m == (B+4) / 2
   n*m == W + B
  */

  int B, W;
  cin >> B >> W;
  int S = (B + 4) / 2;
  for (int n = 1; n <= S; ++n) {
    int m = S - n;
    if (n >= m && n * m == W + B) {
      cout << n << " " << m << endl;
    }
  }
}
