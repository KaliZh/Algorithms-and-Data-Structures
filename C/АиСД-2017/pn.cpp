#include <iostream>
#include <cmath>
#include <cstdio>

using namespace std;

bool prime(int n) {
    for (int i = 2; i < sqrt(n); i++)
        if(n % i == 0)
            return false;
    return true;
}
int main() {
	//freopen("in", "r", stdin);
	//freopen("out", "w", stdout);
    int s = 0;
    for (long i = 2; i <= sqrt(186461322960); i++)
        if (186461322960 % i == 0 &&  prime(i))
            s += i;
    cout << s;

	return 0;
}
