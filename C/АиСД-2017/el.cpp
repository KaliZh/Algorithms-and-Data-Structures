#include <iostream>
#include <cstdio>
#include <cmath>
#include <iomanip>

using namespace std;

double xO, yO, rX, rY;
double ellipse(double x) {
    return sqrt((1 - (pow(x - xO, 2) / rX)) * rY) + yO;
}

int main() {
    int x, y;
    cin >> x >> y;
    cin >> xO >> yO;
    cin >> rX >> rY;
    for (int i = 1; i <= x; i++) { 
        cout << i << " ";
        cout << fixed;
        cout << setprecision(5);
        cout << ellipse(i * 1.0) << "\n";
    }

	return 0;
}
