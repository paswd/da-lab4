#include "z-search.h"

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "converter.h"

using namespace std;

Position::Position(void) {
	this->Line = 1;
	this->Pos = 1;
}
Position::~Position(void) {

}
void Position::Next(void) {
	this->Pos++;
}
void Position::NewLine(void) {
	this->Pos = 1;
	this->Line++;
}

//Classes
ZBlock::ZBlock(void) {
	this->Left = 0;
	this->Right = 0;
	this->Empty = true;
}
void ZBlock::SetNewRange(size_t left, size_t right) {
	this->Empty = false;
	this->Left = left;
	this->Right = right;
}

Mem::Mem(void) {
	this->Cache = 0;
	this->IsRead = false;
	this->LastRead = 0;
}
Mem::~Mem(void) {
}
void Mem::Read(TNumber in) {
	this->Cache = in;
	if (!this->IsRead) {
		this->IsRead = true;
	} else {
		this->LastRead++;
	}
		//this->LastRead++;
}
bool Mem::IsCurrent(size_t pos) {
	//cout << "IsRead: " << (IsRead ? "true" : "false") << endl;
	//cout << "TEST::LastRead: " << this->LastRead << endl;
	//cout << "TEST::Pos: " << pos << endl; 
	return this->LastRead == pos && IsRead;
}
TNumber Mem::GetValue(void) {
	//return this->LastRead;
	return this->Cache;
}

//Functions

vector <TNumber> GetSample(void) {
	string str = "";
	getline(cin, str);
	vector <TNumber> res(1);
	size_t res_size = 0;
	bool is_first_separator = true;
	TNumber tmp = 0;
	bool is_unused = false;
	for (size_t i = 0; i < str.size(); i++) {
		if (IsSeparator(str[i])) {
			if (is_first_separator) {
				is_first_separator = false;
				is_unused = false;
				if (res_size >= res.size()) {
					res.resize(res.size() * 2);
				}
				size_t now_pos = res_size;
				res_size++;
				res[now_pos] = tmp;
				tmp = 0;
			}
			continue;
		}
		is_unused = true;
		is_first_separator = true;
		tmp *= 10;
		tmp += (TNumber) CharToUNum(str[i]);
	}
	if (is_unused) {
		if (res_size >= res.size()) {
			res.resize(res.size() * 2);
		}
		size_t now_pos = res_size;
		res_size++;
		res[now_pos] = tmp;
	}
	res.resize(res_size + 1);
	res[res_size] = -1;
	return res;
}

bool GetNextElement(TNumber *num, bool *is_empty_line, queue <Position> *coordinates, Position *coord_current,
		Mem *mem, TNumber first) {
	int sym;
	bool is_got = false;
	TNumber res = 0;
	*num = 0;
	//*is_end_line = false;
	*is_empty_line = false;
	//*is_last_eol = false;
	bool is_last_eol = false;
	while (true) {
		//cout << "Point" << endl;
		sym = getchar();
		if (IsSeparator(sym) || sym == EOF) {
			if (sym == '\n') {
				//*is_end_line = true;
				if (!is_got) {
					*is_empty_line = true;
					coord_current->NewLine();
					return true;
				} else {
					is_last_eol = true;
				}
			}
			if (!is_got) {
				if (sym == EOF) {
					return false;
				}
				continue;
			}
			if (res == first) {
				coordinates->push(*coord_current);
			}
			if (is_last_eol) {
				coord_current->NewLine();
			} else {
				coord_current->Next();
			}
			*num = res;
			mem->Read(*num);
			return true;
		}
		is_got = true;
		res *= 10;
		res += (TNumber) CharToUNum(sym);
	}
}

vector <size_t> GetZBasic(vector <TNumber> sample) {
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
	/*cout << "SampleZ: ";
	for (size_t i = 0; i < z_function.size(); i++) {
		cout << z_function[i] << ' ';
	}
	cout << endl;*/
	return z_function;
}


void ZSearch(vector <TNumber> in_sample) {
	//vector <size_t> z_function(1); //For testing
	//size_t z_test_size = 0; //For testing


	Sample sample;
	sample.Arr = in_sample;
	//cout << "Sample size: " << sample.Arr.size() << endl;
	sample.ZFunction = GetZBasic(sample.Arr);


	//Cache cache;
	ZBlock block;

	//size_t line_pos = 1;
	//size_t line = 0;
	size_t pos = 0;

	size_t z_current = 0;

	size_t sample_size = sample.Arr.size() - 1;
	//TNumber num = 0;
	bool is_eof = false;
	Mem mem;
	queue <Position> coordinates;
	Position coord_current;
	coord_current.Line = 1;
	coord_current.Pos = 1;

	while (!is_eof || pos <= block.Right) {
		//bool is_last_eol;
		//size_t empty_lines = 0;
		if ((pos > block.Right || block.Empty) && !is_eof) {
			z_current = SearchBasic(sample, pos, &block, &mem, &is_eof, &coordinates, &coord_current);
		} else {
			z_current = SearchInBlock(sample, pos, &block, &mem, &is_eof, &coordinates, &coord_current);
		}
		//cout << "Z_current: " << z_current << ' ';
		//For testing START
		/*if (z_test_size >= z_function.size()) {
			z_function.resize(z_function.size() * 2);
		}
		z_function[z_test_size] = z_current;
		z_test_size++;*/
		//For testing END

		//if (cache.NewLinesCnt(pos) > 0) {
		//cout << "EmptyLines: " << empty_lines << endl;
		/*if (empty_lines > 0) {
			//cout << pos << ":" << cache.NewLinesCnt(pos) << endl;
			line_pos = 1;
			//line += cache.NewLinesCnt(pos);
			line += empty_lines;
		} else {
			line_pos++;
		}*/
		Position res;
		if (z_current > 0) {
			res = coordinates.front();
			coordinates.pop();
		}

		if (z_current >= sample_size) {
			//cout << "~~~~~~~~~~~~~~~~" << endl;
			cout << res.Line << ", " << res.Pos << endl;
			//cout << "~~~~~~~~~~~~~~~~" << endl;
		}
		pos++;

		//cout << "-------" << endl;
		//cout << "Left: " << block.Left << endl;
		//cout << "Right: " << block.Right << endl;
	}
	//For testing START
	/*
	for (size_t i = 0; i < z_test_size; i++) {
		cout << z_function[i] << ' ';
	}
	cout << endl;
	*/
	//For testing END
}

size_t SearchBasicFrom(Sample sample, size_t pos, ZBlock *block, Mem *mem, size_t from,
		bool *is_eof, queue <Position> *coordinates, Position *coord_current) {
	TNumber current = 0;
	//bool is_end_line = false;
	//*is_last_eol = false;
	size_t z_value = from;
	bool is_first = true;
	while (true) {
		//cout << "Pos: " << pos + z_value << endl;
		//cout << "ClearPos: " << pos << endl;
		//cout << "CachePos: " << mem->LastRead << endl;
		//cout << "Block: " << block->Left << ":" << block->Right << endl;
		if (mem->IsCurrent(pos + z_value) && is_first) {
			is_first = false;
			current = mem->GetValue();
			//cout << "FROM::CACHE" << endl;
		} else {
			is_first = false;
			//cout << "FROM::INPUT" << endl;
			//size_t new_lines = 0;
			bool is_empty_line = true;
			/*if (*is_last_eol) {
				//cout << "new_lines1++" << endl;
				new_lines++;
			}*/
			TNumber tmp_num;
			//coord_current->Next();
			while (is_empty_line) {
				*is_eof = !GetNextElement(&tmp_num, &is_empty_line, coordinates, coord_current,
					mem, sample.Arr[0]);
				//read_new = true;
				//cout << "Read()" << endl;
				if (*is_eof) {
					break;
				}
				/*if (is_empty_line) {
					//cout << "new_lines2++" << endl;
					//new_lines++;
					//coord_current->NewLine();
				}*/
			}
			if (*is_eof) {
				break;
			}
			//*empty_lines = new_lines;
			current = tmp_num;
			/*if (current == sample.Arr[0]) {
				coordinates->push(*coord_current);
			}*/
		}
		if (from >= sample.Size()) {
			break;
			if (pos + z_value > 0) {
				//cout << "SetNewRange1: " << pos << " " << z_value << " " << pos + z_value - 1 << endl;
				block->SetNewRange(pos, max(block->Right, pos + z_value - 1));
				//block->Right = pos + z_value - 1;
			}
		}
		//cout << "NowZ: " << z_value << endl;
		//cout << current << ":" << sample.Arr[z_value] << endl;
		if (current == sample.Arr[z_value]) {
			//z_value++;
			if (pos + z_value > 0) {
				//block->Right = max(block->Right, pos + z_value);
				//cout << "SetNewRange2: " << pos << " " << z_value << " " << pos + z_value << endl;
				block->SetNewRange(pos, max(block->Right, pos + z_value));
			}
			//cout << "Z_Increment" << endl;
			z_value++;
			if (sample.IsMatch(z_value)) {
				break;
			} 
		} else {
			//block->Right = pos + z_value - 1;
			break;
		}
		//cout << "-----" << endl;
	}
	//cout << "=====" << endl;
	return z_value;
}

size_t SearchBasic(Sample sample, size_t pos, ZBlock *block, Mem *mem,
		bool *is_eof, queue <Position> *coordinates, Position *coord_current) {
	//cout << "BASIC" << endl;
	//cout << pos << endl;
	return SearchBasicFrom(sample, pos, block, mem, 0, is_eof, coordinates, coord_current);
}
size_t SearchInBlock(Sample sample, size_t pos, ZBlock *block, Mem *mem,
		bool *is_eof, queue <Position> *coordinates, Position *coord_current) {
	//cout << "IN_BLOCK" << endl;
	//cout << pos << endl;
	size_t pre_value = sample.ZFunction[pos - block->Left];
	if (pos + pre_value <= block->Right || pre_value == 0 || *is_eof) {
		//cout << "Return Pre value: " << pre_value << endl;
		//cout << "Block: [" << block->Left << ":" << block->Right << "]" << endl;
		return pre_value;
	} else {
		//cout << "PTT:RIGHT = " << block->Right << endl;
		size_t start_value = block->Right - pos + 1;
		size_t bas_search = SearchBasicFrom(sample, pos, block, mem, start_value,
			is_eof, coordinates, coord_current);
		return max(pre_value, bas_search);
	}
}
