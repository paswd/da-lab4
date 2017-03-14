#include "z-search.h"

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <sstream>
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
}
bool Mem::IsCurrent(size_t pos) {
    return this->LastRead == pos && IsRead;
}
TNumber Mem::GetValue(void) {
    return this->Cache;
}

//Functions

void GetSample(vector <TNumber> *res) {
    string str = "";
    getline(cin, str);
    res->resize(1);
    size_t res_size = 0;
    bool is_first_separator = true;
    TNumber tmp = 0;
    bool is_unused = false;
    for (size_t i = 0; i < str.size(); i++) {
        if (IsSeparator(str[i])) {
            if (is_first_separator) {
                is_first_separator = false;
                is_unused = false;
                if (res_size >= res->size()) {
                    res->resize(res->size() * 2);
                }
                size_t now_pos = res_size;
                res_size++;
                (*res)[now_pos] = tmp;
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
        if (res_size >= res->size()) {
            res->resize(res->size() * 2);
        }
        size_t now_pos = res_size;
        res_size++;
        (*res)[now_pos] = tmp;
    }
    res->resize(res_size + 1);
    (*res)[res_size] = -1;
}

bool GetNextElement(TNumber *num, bool *is_empty_line, queue <Position> *coordinates, Position *coord_current,
        Mem *mem, TNumber first) {
    int sym;
    bool is_got = false;
    *num = 0;
    *is_empty_line = false;
    bool is_last_eol = false;
    string res_str = "";
    while (true) {
        sym = getchar();
        if (IsSeparator(sym) || sym == EOF) {
            if (sym == '\n') {
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
            stringstream sstr(res_str);
            sstr >> *num;
            if (*num == first) {
                coordinates->push(*coord_current);
            }
            if (is_last_eol) {
                coord_current->NewLine();
            } else {
                coord_current->Next();
            }

            mem->Read(*num);
            return true;
        }
        is_got = true;
        res_str += sym;
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
    }
    return z_function;
}


void ZSearch(void) {
    Sample sample;
    GetSample(&(sample.Arr));
    sample.ZFunction = GetZBasic(sample.Arr);

    ZBlock block;
    size_t pos = 0;
    size_t z_current = 0;
    size_t sample_size = sample.Arr.size() - 1;
    bool is_eof = false;
    Mem mem;
    queue <Position> coordinates;
    Position coord_current;
    coord_current.Line = 1;
    coord_current.Pos = 1;

    while (!is_eof || pos <= block.Right) {
        if ((pos > block.Right || block.Empty) && !is_eof) {
            z_current = SearchBasic(&sample, pos, &block, &mem, &is_eof, &coordinates, &coord_current);
        } else {
            z_current = SearchInBlock(&sample, pos, &block, &mem, &is_eof, &coordinates, &coord_current);
        }
        Position res;
        if (z_current > 0) {
            res = coordinates.front();
            coordinates.pop();
        }

        if (z_current >= sample_size) {
            cout << res.Line << ", " << res.Pos << endl;
        }
        pos++;
    }
}

size_t SearchBasicFrom(Sample *sample, size_t pos, ZBlock *block, Mem *mem, size_t from,
        bool *is_eof, queue <Position> *coordinates, Position *coord_current) {
    TNumber current = 0;
    size_t z_value = from;
    bool is_first = true;
    while (true) {
        if (mem->IsCurrent(pos + z_value) && is_first) {
            is_first = false;
            current = mem->GetValue();
        } else {
            is_first = false;
            bool is_empty_line = true;
            TNumber tmp_num;
            while (is_empty_line) {
                *is_eof = !GetNextElement(&tmp_num, &is_empty_line, coordinates, coord_current,
                    mem, sample->Arr[0]);
                if (*is_eof) {
                    break;
                }
            }
            if (*is_eof) {
                break;
            }
            current = tmp_num;
        }
        if (from >= sample->Size()) {
            break;
            if (pos + z_value > 0) {
                block->SetNewRange(pos, max(block->Right, pos + z_value - 1));
            }
        }
        if (current == sample->Arr[z_value]) {
            if (pos + z_value > 0) {
                block->SetNewRange(pos, max(block->Right, pos + z_value));
            }
            z_value++;
            if (sample->IsMatch(z_value)) {
                break;
            } 
        } else {
            break;
        }
    }
    return z_value;
}

size_t SearchBasic(Sample *sample, size_t pos, ZBlock *block, Mem *mem,
        bool *is_eof, queue <Position> *coordinates, Position *coord_current) {
    return SearchBasicFrom(sample, pos, block, mem, 0, is_eof, coordinates, coord_current);
}
size_t SearchInBlock(Sample *sample, size_t pos, ZBlock *block, Mem *mem,
        bool *is_eof, queue <Position> *coordinates, Position *coord_current) {
    size_t pre_value = sample->ZFunction[pos - block->Left];
    if (pos + pre_value <= block->Right || pre_value == 0 || *is_eof) {
        return pre_value;
    } else {
        size_t start_value = block->Right - pos + 1;
        size_t bas_search = SearchBasicFrom(sample, pos, block, mem, start_value,
            is_eof, coordinates, coord_current);
        return max(pre_value, bas_search);
    }
}
