#include <iostream>
#include <cstdio>

using namespace std;

int main() {
	//freopen("in", "r", stdin);
	//freopen("out", "w", stdout);
    int n, a[101], c = 0;
    cin >> n;
    for (int i = 0; i < n; i++)
        cin >> a[i];
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (a[i] < a[j] && (a[i] + a[j]) % 3 == 0)
                c++;
    cout << c;

	return 0;
}
