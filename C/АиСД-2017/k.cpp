#include <iostream>
#include <cstdio>

using namespace std;

int main() {
	//freopen("in", "r", stdin);
	//freopen("out", "w", stdout);
    int a[101] = {0};
    for (int i = 1; i <= 6; i++)
        for (int j = 1; j <= 6; j++)
                a[i + j]++;
    for (int i = 1; i <= 101; i++)
        if (a[i])
            cout << i << ":" << a[i] << "\n";

	return 0;
}
