#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int n = 4;
	cin >> n;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < i; j++)
			cout << '*';
		cout << '\\';
		for (int j = 0; j < (4 * n) - 2 - (2 * i); j++)
			cout << '*';
		cout << '/';
		for (int j = 0; j < i; j++)
			cout << '*';
		cout << endl;
	}

	for (int i = 0; i < (2 * n); i++) {
		for (int j = 0; j < n; j++)
			cout << '*';
		if (i == 0 || i == (n * 2) - 1)
			for (int j = 0; j < 2 * n; j++)
				cout << '#';
		else {
			cout << '|';
			for (int j = 0; j < (4 * n) - 2 - (n * 2); j++)
				cout << '?';
			cout << '|';
		}
		for (int j = 0; j < n; j++)
			cout << '*';
		cout << endl;
	}

	for (int i = n; i < n * 2; i++) {
		for (int j = 0; j < i; j++)
			cout << '*';
		cout << '\\';
		for (int j = 0; j < (4 * n) - 2 - (2 * i); j++)
			cout << '*';
		cout << '/';
		for (int j = 0; j < i; j++)
			cout << '*';
		cout << endl;
	}

	return 0;
}
