#include <iostream>
#include <string>
#include <vector>
//#include "z-search.h"

using namespace std;

typedef long long TNumber;
const int NOTATION_BASE = 10;
const TNumber NUM_EOF = -2;
const TNumber SEPARATOR = -1;

class TTextPosition {
public:
	size_t Line;
	size_t Position;

	void Print() {
		cout << this->Line << ", " << this->Position << endl;
	}
};


TNumber GetNum(bool *is_end_line, bool *is_end_file) {
	//cout << "Get::In" << endl;
	string str_tmp = "";
	int sym;
	*is_end_line = true;
	*is_end_file = true;
	size_t cnt = 0;
	//cout << "Get::StartWhile" << endl;
	while ((sym = getchar()) != EOF) {
		//cout << "Sym: " << sym << endl;
		//cout << cnt << endl;
		cnt++;
		//cout << "GetPoint" << endl;
		if (sym == ' ' || sym == '\n') {
			*is_end_file = false;
			if (sym == ' ') {
				*is_end_line = false;
			}
			break;
		}
		str_tmp += (char) sym;
	}
	//cout << "Get::OutPrev" << endl;
	if (sym == EOF && cnt == 0) {
		return NUM_EOF;
	}
	size_t tmp;
	//cout << "Get::Out" << endl;
	return stoll(str_tmp, &tmp, NOTATION_BASE);
}

int main() {
	string str_tmp;
	getline(cin, str_tmp, '\n');
	vector <TNumber> sample;
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
	//sample.resize(i);
	size_t sample_size = i;
	
	i++;
	sample.resize(i);
	sample[i - 1] = SEPARATOR;
	
	vector <size_t> z_function;
	z_function.resize(sample.size());
	size_t z_block_left = 0;
	size_t z_block_right = 0;
	for (size_t j = 1; j < z_function.size(); j++) {
		if (j <= z_block_right) {
			z_function[j] = min(z_function[j - z_block_left], z_block_right - j + 1);
		} else {
			z_function[j] = 0;
			for (size_t k = j; sample[k] == sample[k - j] && k < sample.size(); k++) {
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
	cout << endl << endl;


	TNumber tmp;
	size_t current_line = 1;
	size_t current_position = 1;
	
	z_block_left = 0;
	z_block_right = 0;
	bool is_end_line;
	bool is_end_file;
	size_t last_got = 0;

	vector <TTextPosition> text_position;
	text_position.resize(1);
	while (true) {
		z_function[i] = 0;
		for (size_t j = i; !is_end_file; j++) {
			if (j >= sample.size()) {
				size_t tmp_size = sample.size() * 2;
				sample.resize(tmp_size);
				z_function.resize(tmp_size);
			}
			size_t position_new_size = j - sample_size - 1;
			if (position_new_size >= text_position.size()) {
				text_position.resize(text_position.size() * 2);
			}

			if (j > last_got) {
				tmp = GetNum(&is_end_line, &is_end_file);
				text_position[position_new_size].Line = current_line;
				text_position[position_new_size].Position = current_position;

				if (!is_end_line) {
					current_position++;
				} else {
					current_line++;
					current_position = 1;
				}

				if (is_end_file) {
					break;
				}

				sample[j] = tmp;
				last_got = j;
			}

			if (sample[j] != sample[j - i]) {
				break;
			}
			z_function[i]++;
		}
		if (is_end_file) {
			break;
		}
		if (z_function[i] == sample_size) {
			text_position[i - sample_size - 1].Print();
		}
		if (z_function[i] > 0) {
			z_block_left = i;
			z_block_right = i + z_function[i] - 1;
		}
		i++;

		long long z_tmp = -1;
		while (i <= z_block_right) {
			z_function[i] = min(z_function[i - z_block_left], z_block_right - i + 1);
			if (z_function[i] == sample_size) {
				text_position[i - sample_size - 1].Print();
			}
			z_tmp = i + z_function[i] - z_block_right - 1;
			if (z_tmp >= 0) {
				z_block_right = i - 1;
				break;
			}
			i++;
		}
		if (z_tmp < 0) {
			i++;
		}
	}

	cout << "Sample: " << sample_size << endl;
	cout << "Total: " << i << endl;
	for (size_t j = sample_size + 1; j < i; j++) {
		cout << z_function[j] << ' ';
	}
	cout << endl;
	
	
	return 0;
}
