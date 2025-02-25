//
//  Mul
// @author maxheap
//
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

int main() {
  long long a, b, c;
  cin >> a >> b >> c;
  if(a == 0 || b == 0) {
      cout << (c == 0 ? "Yes": "No") << endl;
      return 0;
  }
  // a * b <= c
  // a <= c / b + (c % b)
  if (a >= (c / b) + (c % b ? 1: 0)) {
    cout << "Yes" << endl;
  } else {
    cout << "No" << endl;
  }
}
