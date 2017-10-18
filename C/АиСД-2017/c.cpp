#include <cmath>
#include <iostream>
#include <cstdio>

using namespace std;

int main() {
    double dd = 1, x = 0;
    while (dd) {
        cin >> dd;
        x += (17.484 - dd);
    }
    x /= (18 * 17);
    cout << sqrt(x);

	return 0;
}
