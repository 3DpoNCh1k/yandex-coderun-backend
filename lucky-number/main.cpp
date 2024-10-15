#include <cassert>
#include <iostream>
#include <string>

using namespace std;

string AddOne(string s) {
  int n = s.size();
  int first = n - 1;
  while (first >= 0 && s[first] == '9') {
    first--;
  }
  if (first == -1) {
    return string(n, '0');
  }
  s[first] += 1;
  for (int i = first + 1; i < n; ++i) {
    s[i] = '0';
  }
  return s;
};

int CalculateSum(const string &s) {
  int sum = 0;
  for (auto ch : s) {
    sum += ch - '0';
  }
  return sum;
};

string Solve(string s) {
  int n = s.size();

  auto first_half = s.substr(0, n / 2);
  auto second_half = s.substr(n / 2);

  auto s1 = CalculateSum(first_half);
  auto s2 = CalculateSum(second_half);

  if (s1 == s2) {
    return s;
  }
  if (s1 < s2) {
    int diff = s2 - s1;
    int n2 = second_half.size();
    int sum = s2;
    for (int i = n2 - 1; i >= 0; --i) {
      int digit = second_half[i] - '0';
      if (digit < 9) {
        int new_prefix = sum + 1;
        if (s1 >= new_prefix) {
          auto diff = s1 - new_prefix;
          for (int j = n2 - 1; j > i; --j) {
            int add = min(diff, 9);
            second_half[j] = '0' + add;
            diff -= add;
          }
          second_half[i] += 1;
          return first_half + second_half;
        }
      }
      sum -= digit;
    }
    return Solve(AddOne(first_half) + string(n2, '0'));
  }
  int diff = s1 - s2;
  int n2 = second_half.size();
  for (int i = n2 - 1; i >= 0; --i) {
    int add = min(diff, '9' - second_half[i]);
    second_half[i] += add;
    diff -= add;
  }
  return first_half + second_half;
}

int main() {
  std::string s;
  cin >> s;
  int n = s.size();
  auto answer = Solve(AddOne(s));
  if (answer == string(n, '0')) {
    answer = Solve(AddOne(answer));
  }
  assert(CalculateSum(answer.substr(0, n / 2)) ==
         CalculateSum(answer.substr(n / 2)));
  cout << answer << endl;
}