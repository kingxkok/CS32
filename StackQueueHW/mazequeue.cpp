#include <string>
#include <iostream>
#include <queue>
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

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	queue<Coord> coordQueue;

	Coord start(sr, sc);
	coordQueue.push(start);
	maze[sr][sc] = 's';

//	int n = 0;


	while (!coordQueue.empty()) {

		

		Coord& current = coordQueue.front();
		coordQueue.pop();
		const int currentR = current.r();
		const int currentC = current.c();

		//toVisualize
	//	n++;
	//	cout << n << endl;
	//	for (int i = 0; i < 10; i++) cout << maze[i] << endl;
	//	cout << currentR << "," << currentC << endl<< endl;
		



		if (currentR == er && currentC == ec) return true; //if exit, return true

		//NORTH
		 if (maze[currentR - 1][currentC] == '.') {
			maze[currentR - 1][currentC] = '/';
			Coord movedTo(currentR - 1, currentC);
			coordQueue.push(movedTo);
		}

		//EAST
		 if (maze[currentR][currentC + 1] == '.') {
			maze[currentR][currentC + 1] = '/';
			Coord movedTo(currentR, currentC + 1);
			coordQueue.push(movedTo);
		}

		//SOUTH
		 if (maze[currentR + 1][currentC] == '.') {
			maze[currentR + 1][currentC] = '/';
			Coord movedTo(currentR + 1, currentC);
			coordQueue.push(movedTo);
		}

		//WEST
		 if (maze[currentR][currentC - 1] == '.') {
			maze[currentR][currentC - 1] = '/';
			Coord movedTo(currentR, currentC - 1);
			coordQueue.push(movedTo);
		}

		
	}

	return false;
}