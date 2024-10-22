/*
1 -> 1
2 -> 1
3 -> 2
4 -> 2
5 -> 4
6 -> 2
7 -> 6
8 -> 4
9 -> 6
10 -> 4

1, 1, 2, 2, 4, 2, 6, 4, 6, 4

https://oeis.org/search?q=1%2C1%2C2%2C2%2C4%2C2%2C6%2C4%2C6%2C4
*/

#include <iostream>

int phi(int n) {
  int result = n;
  for (int i = 2; i * i <= n; ++i)
    if (n % i == 0) {
      while (n % i == 0)
        n /= i;
      result -= result / i;
    }
  if (n > 1)
    result -= result / n;
  return result;
}

using namespace std;

int main() {
  int n;
  cin >> n;
  cout << phi(n) << endl;
}