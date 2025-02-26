#include <iostream>
#include <memory>
#include <array>
#include <map>
#include <utility>
#include <vector>

using namespace std;

using pii = pair<int, int>;

const int BOUND = (1 << 30) - 1;

class QueryEngine {
private:
    struct Node {
        pii bounds_x;
        pii bounds_y;
        array<int, 4> price = defaultPrice();
        unique_ptr<Node> subdivisions[4];

        // Constructs a Node with the given x and y bounds.
        Node(pii  bx, pii by) : bounds_x(std::move(bx)), bounds_y(std::move(by)) {}

        // Checks if the node is completely within the rectangle defined by bx and by.
        [[nodiscard]] bool within(const pii& bx, const pii& by) const {
            return bx.first <= bounds_x.first && bx.second >= bounds_x.second &&
                   by.first <= bounds_y.first && by.second >= bounds_y.second;
        }

        // Checks if the node intersects with the rectangle defined by bx and by.
        [[nodiscard]] bool intersect(const pii& bx, const pii& by) const {
            return !(bx.second < bounds_x.first || bx.first > bounds_x.second ||
                     by.second < bounds_y.first || by.first > bounds_y.second);
        }

        // Determines the index of the quadrant in which the point (x, y) falls.
        [[nodiscard]] int index(int x, int y) const {
            int mid_x = (bounds_x.first + bounds_x.second) / 2;
            int mid_y = (bounds_y.first + bounds_y.second) / 2;
            return (y > mid_y) * 2 + (x > mid_x);
        }

        // Subdivides the node at the specified quadrant index by creating a child node for that quadrant.
        void subdivide(int index) {
            int mid_x = (bounds_x.first + bounds_x.second) / 2;
            int mid_y = (bounds_y.first + bounds_y.second) / 2;

            if (index == 0) subdivisions[0] = make_unique<Node>(pii{bounds_x.first, mid_x}, pii{bounds_y.first, mid_y});
            if (index == 1) subdivisions[1] = make_unique<Node>(pii{mid_x + 1, bounds_x.second}, pii{bounds_y.first, mid_y});
            if (index == 2) subdivisions[2] = make_unique<Node>(pii{bounds_x.first, mid_x}, pii{mid_y + 1, bounds_y.second});
            if (index == 3) subdivisions[3] = make_unique<Node>(pii{mid_x + 1, bounds_x.second}, pii{mid_y + 1, bounds_y.second});
        }
    };

    unique_ptr<Node> root;
    int property = 0;

public:
    // Initializes the QueryEngine with a root node covering the entire coordinate space.
    QueryEngine() : root(make_unique<Node>(pii{0, BOUND}, pii{0, BOUND})) {}

    // Inserts a property with the given name, coordinates (x, y), and price p into the query engine.
    void insert(const string& name, int x, int y, int p) {
        Node* current = root.get();

        current->price = min(current->price, {p, property, x, y});

        while (current->bounds_x.first != current->bounds_x.second) {
            int idx = current->index(x, y);
            if (!current->subdivisions[idx])
                current->subdivide(idx);
            current = current->subdivisions[idx].get();
            current->price = min(current->price, {p, property, x, y});
        }

        property++;
    }

    // Queries the tree for the cheapest property within the rectangular region defined by bx and by.
    [[nodiscard]] array<int, 4> query(const pii& bx, const pii& by) const {
        return queryRecursive(bx, by, root.get());
    }

private:
    // Recursively searches for the cheapest property within the specified rectangular region.
    static array<int, 4> queryRecursive(const pii& bx, const pii& by, const Node* node) {
        if (!node || !node->intersect(bx, by)) return defaultPrice();
        if (node->within(bx, by)) return node->price;

        array<int, 4> result = defaultPrice();
        for (const auto& sub : node->subdivisions) {
            if (sub) {
                result = min(result, queryRecursive(bx, by, sub.get()));
            }
        }
        return result;
    }

    // Returns a default price array indicating that no valid property was found.
    static array<int, 4> defaultPrice() {
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
        engine.insert(name, x, y, p);
    }

    int q; cin >> q;

    while (q--) {
        int x1, x2, y1, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        auto [price, _, x, y] = engine.query({x1, x2}, {y1, y2});

        if (property_name.count({x, y})) cout << property_name[{x, y}] << endl;
        else cout << "No property found in this region" << endl;
    }
}