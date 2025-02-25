#include <bits/stdc++.h>

using namespace std;

struct Line {
    mutable long long k, m, p;
    bool operator<(const Line& o) const { return k < o.k; }
    bool operator<(long long x) const { return p < x; }
};

struct LineContainer : multiset<Line, less<>> {
    static const long long inf = LLONG_MAX;
    long long div(long long a, long long b) {
        return a / b - ((a ^ b) < 0 && a % b);
    }
    bool isect(iterator x, iterator y) {
        if (y == end()) return x->p = inf, 0;
        if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
        else x->p = div(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }
    void add(long long k, long long m) {
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p)
            isect(x, erase(y));
    }
    long long query(long long x) {
        assert(!empty());
        auto l = *lower_bound(x);
        return abs(l.k * x + l.m);
    }
};

int main() {
    LineContainer lines;
    int Q; cin >> Q;
    while(Q--) {
        char op; cin >> op;
        if (op == 'A') {
            int k, m; cin >> k >> m;
            lines.add(k, m);
        } else {
            int x; cin >> x;
            long long maxDist = lines.query(x);
            cout << maxDist << endl;
        }
    }
}