#include <iostream>
#include <cstdlib>
#include "../backend/field.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		cout << "Usage: " << argv[0] << " size count" << endl;
		return 1;
	}
	int size = atoi(argv[1]),
		count = atoi(argv[2]);

	if (!size || !count)
	{
		cout << "Invalid size or count\n";
		return 1;
	}

	int countGood = 0;
	double avgWallCount = 0.0;
	for (int i = 0; i<count; ++i)
	{
		Field *field = new Field(size, size);
		if (field->getCell(size-1, size-1)==EMPTY) ++countGood;
		for (int j = 0; j<size; ++j)
			for (int k = 0; k<size; ++k)
				avgWallCount += field->getCell(j, k)==WALL;
		if (i) avgWallCount /= 2;

		delete field;
	}
	cout << "The test passed " << ((double)countGood)/count*100 << "% of the times" << endl;
	cout << "Average wall percentage: " << ((double)avgWallCount)/(size*size)*100 << "%" << endl;
}
