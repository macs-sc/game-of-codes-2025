#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll findParentStation(ll x) {
    if (x == 2) return 1;
    ll currentStation = 1;
    ll firstChildInLevel = 2;
    while (firstChildInLevel <= x) {
        ll childrenCount = currentStation;
        ll lastChildInLevel = firstChildInLevel + childrenCount - 1;
        if (x >= firstChildInLevel && x <= lastChildInLevel) {
            return currentStation;
        }
        currentStation++;
        firstChildInLevel = lastChildInLevel + 1;
    }
    return -1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    ll x;
    cin >> x;
    cout << findParentStation(x) << '\n';
}