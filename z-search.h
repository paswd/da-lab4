#ifndef Z_SEARCH_H
#define Z_SEARCH_H

#include <string>
#include <vector>
#include <queue>

typedef long long TNumber;

class ZBlock {
public:
	size_t Left;
	size_t Right;
	bool Empty;

	ZBlock(void);
	void SetNewRange(size_t left, size_t right);
};

/*class Cache {
private:
	std::vector <TNumber> Arr;
	std::vector <size_t> NewLines;
	size_t ArrSize;
	bool IsNextEOL;
public:
	size_t Left;
	size_t Right;

	TNumber Last;
	bool IsLast;

	Cache(void);
	TNumber GetElement(size_t pos);
	size_t NewLinesCnt(size_t pos);
	void Push(TNumber element, size_t new_lines_cnt);
	void Pop(size_t length);
	void Print(void);
	void Clear(void);
	size_t GetSize(void);
	void SetNextEOL(void);
};*/

class Position {
public:
	size_t Line;
	size_t Pos;

	Position(void);
	~Position(void);
	void Next(void);
	void NewLine(void);
};

class Mem {
private:
	TNumber Cache;
	//size_t LastRead;
	bool IsRead;

public:
	size_t LastRead;
	Mem(void);
	~Mem(void);
	void Read(TNumber in);
	bool IsCurrent(size_t pos);
	TNumber GetValue(void);
};

class Sample {
public:
	std::vector <TNumber> Arr;
	std::vector <size_t> ZFunction;

	bool IsMatch(size_t z_current) {
		return z_current >= this->Arr.size() - 1;
	}
	size_t Size(void) {
		return this->Arr.size();
	}
};
/*class Coordinate {
public:
	size_t Line;
	size_t Pos;

	Coordinate(void);
};*/

std::vector <TNumber> GetSample(void);
bool GetNextElement(TNumber *num, bool *is_empty_line, std::queue <Position> *coordinates, Position *coord_current,
		Mem *mem);
std::vector <size_t> GetZBasic(std::vector <TNumber> sample);

size_t SearchBasicFrom(Sample sample, size_t pos, ZBlock *block, Mem *mem, size_t from,
		bool *is_eof, std::queue <Position> *coordinates, Position *coord_current);
size_t SearchBasic(Sample sample, size_t pos, ZBlock *block, Mem *mem,
		bool *is_eof, std::queue <Position> *coordinates, Position *coord_current);
size_t SearchInBlock(Sample sample, size_t pos, ZBlock *block, Mem *mem,
		bool *is_eof, std::queue <Position> *coordinates, Position *coord_current);

void ZSearch(void);

#endif
