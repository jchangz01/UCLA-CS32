/*#include <iostream>
#include <string>
#include <stack>

using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    // Return true if there is a path from (sr,sc) to (er,ec)
    // through the maze; return false otherwise
    if (sr >= nRows || sc >= nCols || sr < 0 || sc < 0 || er >= nRows || ec >= nCols || er < 0 || ec < 0)
        return false;

    stack<Coord> coordStack;
    Coord start(sr, sc);
    coordStack.push(start);
    maze[sr][sc] = 'D';
    while (!coordStack.empty())
    {
        Coord current = coordStack.top();
        coordStack.pop();
        if (current.r() == er && current.c() == ec)
            return true;
        else
        {
            if (maze[current.r()][current.c() + 1] == '.')
            {
                coordStack.push(Coord(current.r(), current.c() + 1));
                maze[current.r()][current.c() + 1] = 'D';
            }
            if (maze[current.r() + 1][current.c()] == '.')
            {
                coordStack.push(Coord(current.r() + 1, current.c()));
                maze[current.r() + 1][current.c()] = 'D';
            }
            if (maze[current.r()][current.c() - 1] == '.')
            {
                coordStack.push(Coord(current.r(), current.c() - 1));
                maze[current.r()][current.c() - 1] = 'D';
            }
            if (maze[current.r() - 1][current.c()] == '.')
            {
                coordStack.push(Coord(current.r() - 1, current.c()));
                maze[current.r() - 1][current.c()] = 'D';
            }
        }
    }
    return false;
}

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X...X..X.X",
        "X.XXX....X",
        "X.X.XXXX.X",
        "XXX......X",
        "X...X.XX.X",
        "X.X.X..X.X",
        "X.XXXX.X.X",
        "X..X...X.X",
        "XXXXXXXXXX"
    };

    if (pathExists(maze, 10, 10, 4, 3, 1, 8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
*/