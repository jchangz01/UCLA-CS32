#include <iostream>
#include "History.h"
#include "globals.h"

using namespace std;

History::History(int nRows, int nCols)
{
	m_Row = nRows;
	m_Col = nCols;
	for (int r = 1; r <= m_Row; r++)
	{
		for (int c = 1; c <= m_Col; c++)
			grid[r][c] = 64;
	}
}

bool History::record(int r, int c )
{
	if (r < 1 || r > m_Row || c < 1 || c > m_Col)
		return false;

	grid[r][c]++;
	return true;
}

void History::display() const
{
	clearScreen();
	for (int r = 1; r <= m_Row; r++)
	{
		for (int c = 1; c <= m_Col; c++)
		{
			if (grid[r][c] < 65)
				cout << '.';
			else if (grid[r][c] >= 90)
				cout << 'Z';
			else
				cout << (char)grid[r][c];
		}
		cout << endl;
	}
	cout << endl;
}
