#include<iostream>

using namespace std;

int main(){
    int t; cin >> t;
    while(t--){
        int n, m; cin >> n >> m;
        if ((m & 1) && (n & 1)) {
            cout << "Zakariaa" << endl;
        } else {
            cout << "Anas" << endl;
        }
    }
}