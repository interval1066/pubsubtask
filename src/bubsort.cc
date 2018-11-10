#include <iostream>
#include <vector>
#include <cstdlib>

void
bubbleSort(std::vector<int>& a)
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

