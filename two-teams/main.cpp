#include <iostream>

using namespace std;

int main() {
  int a, b, n;
  cin >> a >> b >> n;
  cout << (a > ((b + n - 1) / n) ? "Yes" : "No") << endl;
}