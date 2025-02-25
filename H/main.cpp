#include <vector>
#include <map>
#include <set>
#include <iostream>

using namespace std;



class UnionFind {
public:
    map<int, int> parent, rank;

    int find(int i) {
        if (!parent.count(i)) {
            parent[i] = i;
            rank[i] = 0;
        }
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }

    void unite(int i, int j) {
        int pi = find(i), pj = find(j);
        if (pi == pj) return;

        if (rank[pi] < rank[pj]) swap(pi, pj);
        parent[pj] = pi;
        if (rank[pi] == rank[pj]) rank[pi]++;
    }

    bool same(int i, int j) {
        return find(i) == find(j);
    }
};


vector<int> preprocess(int N) {
    vector<int> divisor(N + 1, INT_MAX);
    divisor[0] = divisor[1] = 1;
    for (int i = 2; i <= N; ++i) {
        if (divisor[i] == INT_MAX) {
            for (int j = i; j <= N; j += i) {
                divisor[j] = min(divisor[j], i);
            }
        }
    }
    return divisor;
}

int main() {
    vector<int> divisor = preprocess(1e6);

    int n;
    cin >> n;

    UnionFind dsu;

    for (int i = 0; i < n; i++) {
        int num; cin >> num;
        if (num == 1) {
            cout << "no\n";
            return 0;
        }
        int last = divisor[num];
        while (num != 1) {
            dsu.unite(last, divisor[num]);
            num /= divisor[num];
        }
    }

    set<int> roots;
    for (auto [a, _] : dsu.parent) {
        roots.insert(dsu.find(a));
    }

    cout << (roots.size() == 1 ? "yes" : "no") << endl;
}