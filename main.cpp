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

bool IsSeparator(int sym) {
	if (sym == ' ' || sym == '\t' || sym == '\n') {
		return true;
	}
	return false;
}


TNumber GetNum(bool *is_end_line, size_t *line_shift, bool *is_end_file) {
	//cout << "Get::In" << endl;
	string str_tmp = "";
	int sym;
	*is_end_line = true;
	*is_end_file = true;
	size_t cnt = 0;
	bool is_first = true;
	//cout << "Get::StartWhile" << endl;
	while ((sym = getchar()) != EOF) {
		//cout << "Sym: " << sym << endl;
		//cout << cnt << endl;
		//cout << "GetPoint" << endl;
		if (is_first && IsSeparator(sym)) {
			if (sym == '\n') {
				(*line_shift)++;
			}
			continue;
		}
		cnt++;
		if (!IsSeparator(sym)) {
			is_first = false;
		}
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
		if (!IsSeparator(str_tmp[0])) {
			if (i >= sample.size()) {
				sample.resize(sample.size() * 2);
			}
			sample[i] = stoll(str_tmp, &str_start, NOTATION_BASE);
		} else {
			i--;
			str_start = 1;
		}
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
		//} else {
			if (j + z_function[j] < z_block_right) {
				continue;
			}
		}
		z_function[j] = 0;
		for (size_t k = j; sample[k] == sample[k - j] && k < sample.size(); k++) {
			z_function[j]++;
		}
		if (z_function[j] > 0) {
			z_block_left = j;
			z_block_right = j + z_function[j] - 1;
		}
		//}
	}
	/*for (size_t j = 0; j < z_function.size(); j++) {
		cout << z_function[j] << ' ';
	}
	cout << endl << endl;*/


	TNumber tmp;
	size_t current_line = 1;
	size_t current_position = 1;
	
	z_block_left = 0;
	z_block_right = 0;
	bool is_end_line = false;
	bool is_end_file = false;
	size_t last_got = 0;

	//cout << "Point1" << endl;
	vector <TTextPosition> text_position;
	text_position.resize(1);
	//cout << "Point2" << endl;
	//size_t read = 0;
	while (true) {
		/*bool abc = false;
		if (i == 27) {
			abc = true;
		}*/
		//cout << "Cicle1" << endl;
		if (i >= sample.size()) {
			size_t tmp_size = sample.size() * 2;
			sample.resize(tmp_size);
			z_function.resize(tmp_size);
		}
		z_function[i] = 0;
		for (size_t j = i; !is_end_file; j++) {
			//cout << "Cicle2" << endl;
			//cout << "Point1" << endl;
			if (j >= sample.size()) {
				size_t tmp_size = sample.size() * 2;
				sample.resize(tmp_size);
				z_function.resize(tmp_size);
			}
			//cout << "Point2" << endl;
			size_t position_new_size = j - sample_size - 1;
			if (position_new_size >= text_position.size()) {
				text_position.resize(text_position.size() * 2);
			}
			//cout << "Point3" << endl;

			//cout << "Read: " << j << ":" << last_got << endl;

			if (j > last_got && !is_end_file) {
				size_t line_shift = 0;
				tmp = GetNum(&is_end_line, &line_shift, &is_end_file);
				//read++;
				current_line += line_shift;
				if (line_shift > 0) {
					current_position = 1;
				}
				text_position[position_new_size].Line = current_line;
				text_position[position_new_size].Position = current_position;

				if (!is_end_line) {
					current_position++;
					//cout << "Pos1: " << current_line << ":" << current_position << endl;
				} else {
					current_line++;
					current_position = 1;
					//cout << "Pos2: " << current_line << ":" << current_position << endl;
				}


				if (is_end_file) {
					break;
				}

				last_got = j;
				sample[j] = tmp;
			}
			/*if (j == 27) {
				abc = true;
			}
			if (abc) {
				cout << "J: " << j << endl;
				cout << sample[j] << ":" << sample[j - i] << endl;
				cout << "J | J - I: " << j << " " << j - i << endl;
				cout << endl;
				for (size_t k = 0; k < sample.size(); k++) {
					cout << sample[k] << ' ';
				}
				cout << endl;
				for (size_t k = 0; k < sample.size(); k++) {
					cout << z_function[k] << ' ';
				}
				cout << endl;
			}*/
			if (sample[j] != sample[j - i]) {
				break;
			}
			z_function[i]++;
		}
		/*if (is_end_file && i >= last_got) {
			break;
		}*/
		if (z_function[i] == sample_size) {
			text_position[i - sample_size - 1].Print();
		}
		if (z_function[i] > 0) {
			z_block_left = i;
			z_block_right = i + z_function[i] - 1;
			//cout << "Block: " << z_block_left << " " << z_block_right << endl;
		}
		if (is_end_file && i >= last_got && z_function[i] < sample_size) {
			break;
		}
		
		/*if (!is_end_file || i < last_got) {
			cout << "Inc1: " << i << ":" << last_got << endl;
			i++;
		}*/
		bool is_z_cicle_first = true;

		long long z_tmp = -1;
		while (i <= z_block_right) {
			if (is_z_cicle_first) {
				is_z_cicle_first = false;
				if (is_end_file && i >= last_got) {
					break;
				}
				i++;
				continue;
			}
			//cout << "Cicle3" << endl;
			z_function[i] = min(z_function[i - z_block_left], z_block_right - i + 1);
			if (z_function[i] == sample_size) {
				text_position[i - sample_size - 1].Print();
			}
			z_tmp = i + z_function[i] - z_block_right - 1;
			if (z_tmp >= 0) {
				z_block_right = i - 1;
				break;
			}
			//if (!is_end_file || i < last_got) {
			//cout << "Inc2: " << i << ":" << last_got << endl;
			i++;
				//continue;
			//}
		}
		if (z_tmp < 0 && (!is_end_file || i < last_got) && sample[i] != sample[0]) {
			//cout << "Inc3:" << i << ":" << last_got << endl;
			i++;
		}
		if (is_end_file) {
			break;
		}
		//
		/*for (size_t z = sample_size + 1; z < z_function.size(); z++) {
			cout << z_function[z] << " ";
		}
		cout << endl;*/

		/*if (i >= z_block_right && z_tmp < 0) {		
			z_block_left = 0;
			z_block_right = 0;
			last_got = 0;

			text_position.resize(1);
			sample.resize(sample_size * 2);
			z_function.resize(sample_size * 2);
			i = sample_size + 1;
		}*/
		size_t index_diff = i - sample_size - 1;
		for (size_t k = i; k <= last_got; k++) {
			sample[k - index_diff] = sample[k];
			z_function[k - index_diff] = z_function[k];
			text_position[k - sample_size - 1 - index_diff] = text_position[k - sample_size - 1];
		}
		i -= index_diff;
		last_got -= index_diff;
		z_block_left = 0;
		z_block_right = 0;
	}

	//cout << "Got: " << read - 1 << endl;
	//cout << "Sample: " << sample_size << endl;
	//cout << "Total: " << i << endl;
	/*for (size_t j = sample_size + 1; j < i; j++) {
		cout << z_function[j] << ' ';
	}
	cout << endl;*/
	
	
	return 0;
}
