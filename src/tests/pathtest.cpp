#include <iostream>
#include "../backend/field.h"
#include "shared.h"

using namespace std;

#define COUNT 1000

int main()
{
	int countGood = 0;
	double avgWallCount = 0.0;
	for (int i = 0; i<COUNT; ++i)
	{
		Field *field = new Field(SIZE, SIZE);
		if (field->getCell(SIZE-1, SIZE-1)==EMPTY) ++countGood;
		for (int j = 0; j<SIZE; ++j)
			for (int k = 0; k<SIZE; ++k)
				avgWallCount += field->getCell(j, k)==WALL;
		if (i) avgWallCount /= 2;

		delete field;
	}
	cout << "The test passed " << ((double)countGood)/COUNT*100 << "% of the times" << endl;
	cout << "Average wall percentage: " << ((double)avgWallCount)/(SIZE*SIZE)*100 << "%" << endl;
}
