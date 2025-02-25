#include <iostream>
#include <vector>
#include <set>
#include <numeric>

using namespace std;

vector<int> normalize(const vector<int>& arr) {
    int d = arr[0];
    for (int i = 1; i < (int)arr.size(); ++i) {
        d = gcd(d, arr[i]);
    }
    vector<int> result;
    for (int num : arr) {
        result.push_back(num / d);
    }
    return result;
}

int matrixRank(const vector<vector<int>>& matrix) {
    set<vector<int>> unique_rows;
    for (const auto& row : matrix) {
        unique_rows.insert(normalize(row));
    }
    return unique_rows.size();
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> matrix(n, vector<int>(m));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> matrix[i][j];
        }
    }

    vector<vector<int>> transpose(m, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            transpose[j][i] = matrix[i][j];
        }
    }

    cout << min(matrixRank(matrix), matrixRank(transpose)) << endl;
}