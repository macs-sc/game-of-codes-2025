#include <iostream>

typedef long long ll;

using namespace std;

const ll MOD = 1e9 + 7;

// Function to find the first and last occurrence of '1' in a binary string
pair<int, int> getFirstAndLastOne(const string& s) {
    pair<int, int> res = {INT_MAX, INT_MIN};
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '1') {
            res.first = min(res.first, i);  // Update first occurrence
            res.second = max(res.second, i); // Update last occurrence
        }
    }
    return res;
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        string S, T;
        cin >> S >> T;

        auto [firstOne, lastOne] = getFirstAndLastOne(S);

        // If S contains only '0's, the best way to reach T is by performing 1 increment followed by N - 1 shifts
        if (firstOne == INT_MAX) {
            cout << S.size() << endl;
            continue;
        }

        // If the leftmost bit is '1' and there are other '1's, S cannot be transformed into T
        if (firstOne == 0 && lastOne != 0) {
            cout << -1 << endl;
            continue;
        }

        // If S has exactly one '1', the answer is the number of shifts needed to move it to the start
        if (firstOne == lastOne) {
            cout << firstOne << endl;
            continue;
        }

        // Calculate the minimum increments needed to transform S into T
        // We compute the inverse bits needed to fill all zeros between the first '1' and the end
        ll power = 1, inverse = 0;
        for (int i = (int) S.size() - 1; i >= firstOne; i--) {
            if (S[i] == '0') {
                inverse = (inverse + power) % MOD; // Add contribution of 0-bits
            }
            power = (power * 2) % MOD; // Update power of 2
        }

        // Total operations: filling zeros + shifting left until the first '1' reaches the first position
        cout << (inverse + firstOne) % MOD << endl;
    }
}
