#include <iostream>
#include <string>
#include <vector>
#include "z-search.h"

using namespace std;

const int NOTATION_BASE = 10;

int main() {
	string str_tmp;
	getline(cin, str_tmp, '\n');
	vector <long long> sample;
	sample.resize(1);
	size_t str_start = 0;
	size_t str_end = str_tmp.size();
	size_t i;
	for (i = 0; str_end > 0; i++) {
		if (i >= sample.size()) {
			sample.resize(sample.size() * 2);
		}
		sample[i] = stoll(str_tmp, &str_start, NOTATION_BASE);
		//str_start++;
		//cout << "Input: " << sample[i] << endl;
		//cout << str_start << ":" << str_end << endl;
		str_tmp = str_tmp.substr(str_start, str_end - str_start);
		str_end = str_tmp.size();
		str_start = 0;

	}
	sample.resize(i);
	
	i++;
	sample.resize(i);
	sample[i - 1] = -1;
	
	vector <size_t> z_function;
	z_function.resize(sample.size());
	size_t z_block_left = 0;
	size_t z_block_right = 0;
	for (size_t j = 1; j < z_function.size(); j++) {
		if (j <= z_block_right) {
			z_function[j] = min(z_function[j - z_block_left], z_block_right - j + 1);
		} else {
			z_function[j] = 0;
			for (size_t k = j; sample[k] == sample [k - j] && k < sample.size(); k++) {
				z_function[j]++;
			}
			if (z_function[j] > 0) {
				z_block_left = j;
				z_block_right = j + z_function[j] - 1;
			}
		}
	}
	for (size_t j = 0; j < z_function.size(); j++) {
		cout << z_function[j] << ' ';
	}
	cout << endl;


	/*TNumber tmp;
	int sym;
	str_tmp = 0;
	size_t current_line = 1;
	size_t current_position = 1;
	
	z_block_left = 0;
	z_block_right = 0;
	bool is_first = true;
	while ((sym = getchar()) != EOF) {
		if (sym == ' ' || sym == '\n') {
			i++;
			if (i >= sample.size()) {
				sample.resize(sample.size() * 2);
			}
			sample[i] = stoll(str_tmp, &str_start; NOTATION_BASE);
			str_tmp = "";

			if (!is_first) {
				if (i > z_block_right) {
					if (sample[i] == )
				} else {

				}
			} else {
				is_first = false;
			}

			if (sym == ' ') {
				current_position++;
				continue;
			} else {
				current_line++;
				current_position = 1;
				continue;
			}
		}
		str_tmp += (char) sym;
	}*/
	
	return 0;
}
