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
        vector<array<int, 4>> points; // Stores points in the form (price, order of insertion, x, y)
        unique_ptr<Node> subdivisions[4];
        array<int, 4> price = defaultPrice(); // Stores cheapest price and position within this node

        // Constructs a node with the given x and y bounds.
        Node(const pii& bx, const pii& by) : bounds_x(bx), bounds_y(by) {}

        // Checks if the node is completely within the rectangle defined by bx and by.
        bool within(const pii& bx, const pii& by) const {
            return bx.first <= bounds_x.first && bx.second >= bounds_x.second &&
                   by.first <= bounds_y.first && by.second >= bounds_y.second;
        }

        // Checks if the node intersects with the rectangle defined by bx and by.
        bool intersect(const pii& bx, const pii& by) const {
            return !(bx.second < bounds_x.first || bx.first > bounds_x.second ||
                     by.second < bounds_y.first || by.first > bounds_y.second);
        }

        // Determines which quadrant (subdivision index) the point (x, y) belongs to.
        int index(int x, int y) const {
            int mid_x = (bounds_x.first + bounds_x.second) / 2;
            int mid_y = (bounds_y.first + bounds_y.second) / 2;
            return (y > mid_y) * 2 + (x > mid_x);
        }

        // Subdivides the current node into four child quadrants.
        void subdivide() {
            int mid_x = (bounds_x.first + bounds_x.second) / 2;
            int mid_y = (bounds_y.first + bounds_y.second) / 2;
            subdivisions[0] = make_unique<Node>(pii{bounds_x.first, mid_x}, pii{bounds_y.first, mid_y});
            subdivisions[1] = make_unique<Node>(pii{mid_x + 1, bounds_x.second}, pii{bounds_y.first, mid_y});
            subdivisions[2] = make_unique<Node>(pii{bounds_x.first, mid_x}, pii{mid_y + 1, bounds_y.second});
            subdivisions[3] = make_unique<Node>(pii{mid_x + 1, bounds_x.second}, pii{mid_y + 1, bounds_y.second});
        }

        // Redistributes points from the current node into its subdivisions.
        void redistributePoints() {
            for (auto& [p, order, x, y] : points) {
                int idx = index(x, y);
                subdivisions[idx]->points.push_back({p, order, x, y});
                subdivisions[idx]->updatePrice(p, order, x, y);
            }
            points.clear();
        }

        // Updates the node's stored price if the given point has a cheaper price.
        void updatePrice(int p, int order, int x, int y) {
            price = min(price, {p, order, x, y});
        }
    };

    unique_ptr<Node> root;

public:
    // Initializes the QueryEngine with a root node covering the entire coordinate bounds.
    QueryEngine() : root(make_unique<Node>(pii{0, BOUND}, pii{0, BOUND})) {}

    // Inserts a list of properties into the query engine and balances the tree accordingly.
    void insert(const vector<array<int, 4>>& properties) {
        auto node = root.get();
        node->points = properties;
        balanceTree(root.get());
    }

    // Queries the tree for the cheapest property within the specified rectangular region.
    [[nodiscard]] array<int, 4> query(const pii& bx, const pii& by) const {
        return queryRecursive(bx, by, root.get());
    }

private:
    // Balances the tree by subdividing nodes that exceed the threshold number of points.
    void balanceTree(Node* node) {
        if (node->points.size() <= THRESHOLD) return;

        if (!node->subdivisions[0]) node->subdivide();
        node->redistributePoints();

        for (auto &sub : node->subdivisions) {
            balanceTree(sub.get());
        }
    }

    // Recursively searches for the cheapest property within the specified rectangular region.
    static array<int, 4> queryRecursive(const pii& bx, const pii& by, const Node* node) {
        if (!node || !node->intersect(bx, by)) return defaultPrice();
        if (node->within(bx, by)) return node->price;

        array<int, 4> result = defaultPrice();
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

    // Returns a default price array indicating that no valid property was found.
    [[nodiscard]] static array<int, 4> defaultPrice() {
        return {INT_MAX, INT_MAX, -1, -1};
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
