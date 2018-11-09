#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

void
bubbleSort(vector<int>& a)
{
	bool bSwap = false;

	while(bSwap) {
		for (size_t i = 0; i < a.size() - 1; i++) {
			if (a[i] > a[i + 1] ) {

				a[i] += a[i + 1];
				a[i + 1] = a[i] - a[ i + 1];
				a[i] -= a[i + 1];

				bSwap = true;
			}
		}
	}
}

void
printVector(vector<int> a)
{
	for (size_t i = 0; i < a.size(); i++)
		cout << a[i] << " ";

	cout << endl;
}

