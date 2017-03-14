#include <iostream>
#include <string>
#include <vector>

#include "z-search.h"

using namespace std;

int main(void) {
	vector <TNumber> sample = GetSample();
	ZSearch(sample);

	return 0;
}
