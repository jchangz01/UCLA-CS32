#ifndef HISTORY_H
#define HISTORY_H

#include "globals.h"

class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	int m_Row;
	int m_Col;
	int grid[MAXROWS][MAXCOLS];
};
#endif // !HISTORY_H