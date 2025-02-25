#include <cassert>
#include <iostream>
#include <memory>
#include <array>
#include <map>
#include <vector>

using namespace std;

using pii = pair<int, int>;
const int BOUND = (1 << 30) - 1;
const int THRESHOLD = 64; // Maximum points in a node before subdivision

class QueryEngine {
private:
    struct Node {
        pii bounds_x;
        pii bounds_y;
        vector<array<int, 4>> points; // Stores (x, y, price, order of insertion)
        unique_ptr<Node> subdivisions[4];
        array<int, 4> price = {INT_MAX, INT_MAX, -1, -1}; // Stores cheapest price and position

        Node(const pii& bx, const pii& by) : bounds_x(bx), bounds_y(by) {}

        bool contains(int x, int y) const {
            return x >= bounds_x.first && x <= bounds_x.second &&
                   y >= bounds_y.first && y <= bounds_y.second;
        }

        bool within(const pii& bx, const pii& by) const {
            return bx.first <= bounds_x.first && bx.second >= bounds_x.second &&
                   by.first <= bounds_y.first && by.second >= bounds_y.second;
        }

        bool intersect(const pii& bx, const pii& by) const {
            return !(bx.second < bounds_x.first || bx.first > bounds_x.second ||
                     by.second < bounds_y.first || by.first > bounds_y.second);
        }

        int index(int x, int y) const {
            int mid_x = (bounds_x.first + bounds_x.second) / 2;
            int mid_y = (bounds_y.first + bounds_y.second) / 2;
            return (y > mid_y) * 2 + (x > mid_x);
        }

        void subdivide() {
            int mid_x = (bounds_x.first + bounds_x.second) / 2;
            int mid_y = (bounds_y.first + bounds_y.second) / 2;
            subdivisions[0] = make_unique<Node>(pii{bounds_x.first, mid_x}, pii{bounds_y.first, mid_y});
            subdivisions[1] = make_unique<Node>(pii{mid_x + 1, bounds_x.second}, pii{bounds_y.first, mid_y});
            subdivisions[2] = make_unique<Node>(pii{bounds_x.first, mid_x}, pii{mid_y + 1, bounds_y.second});
            subdivisions[3] = make_unique<Node>(pii{mid_x + 1, bounds_x.second}, pii{mid_y + 1, bounds_y.second});
        }

        void redistributePoints() {
            for (auto& [p, order, x, y] : points) {
                int idx = index(x, y);
                subdivisions[idx]->points.push_back({p, order, x, y});
                subdivisions[idx]->updatePrice(p, order, x, y);
            }
            points.clear();
        }

        void updatePrice(int p, int order, int x, int y) {
            price = min(price, {p, order, x, y});
        }
    };

    unique_ptr<Node> root;
    int size = 0;

public:
    QueryEngine() : root(make_unique<Node>(pii{0, BOUND}, pii{0, BOUND})) {}

    void insert(const vector<array<int, 4>>& properties) {
        auto node = root.get();
        node->points = properties;
        balanceTree(root.get());
        size++;
    }

    [[nodiscard]] array<int, 4> query(const pii& bx, const pii& by) const {
        return queryRecursive(bx, by, root.get());
    }

private:
    void balanceTree(Node* node) {
        if (node->points.size() <= THRESHOLD) return;

        if (!node->subdivisions[0]) node->subdivide();
        node->redistributePoints();

        for (auto &sub : node->subdivisions) {
            balanceTree(sub.get());
        }
    }

    static array<int, 4> queryRecursive(const pii& bx, const pii& by, const Node* node) {
        if (!node || !node->intersect(bx, by)) return {INT_MAX, INT_MAX, -1, -1};
        if (node->within(bx, by)) return node->price;

        array<int, 4> result = {INT_MAX, INT_MAX, -1, -1};
        if (!node->subdivisions[0]) {
            for (const auto& [p, order, x, y] : node->points) {
                if (!(x >= bx.first && x <= bx.second && y >= by.first && y <= by.second)) continue;
                result = min(result, {p, order, x, y});
            }
        } else {
            for (const auto &sub: node->subdivisions) {
                if (sub) {
                    result = min(result, queryRecursive(bx, by, sub.get()));
                }
            }
        }

        return result;
    }

};


int main() {
    QueryEngine engine;
    map<pii, string> property_name;
    vector<array<int, 4>> properties;

    int n; cin >> n;
    for (int i = 0; i < n; i++) {
        string name;
        int x, y, p;
        cin >> name >> x >> y >> p;
        property_name[{x, y}] = name;
        properties.push_back({p, i, x, y});
    }

    engine.insert(properties);

    int q; cin >> q;

    while (q--) {
        int x1, x2, y1, y2; cin >> x1 >> y1 >> x2 >> y2;
        auto [price, _, x, y] = engine.query({x1, x2}, {y1, y2});

        if (property_name.count({x, y})) cout << property_name[{x, y}] << endl;
        else cout << "No property found in this region" << endl;
    }
}
