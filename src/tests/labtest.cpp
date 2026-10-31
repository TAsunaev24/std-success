#include <iostream>
#include <cstdlib>
#include "../backend/field.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		cout << "Usage: labtest size\n";
		return 1;
	}

	int size = atoi(argv[1]);
	if (!size)
	{
		cout << "Invalid size\n";
		return 1;
	}

	Field field = Field(size, size);
	for (int i = 0; i<size; ++i)
	{
		for (int j = 0; j<size; ++j)
			cout << (field.getCell(i, j)==WALL?'O':' ');

		cout << endl;
	}
	return 0; 
}
