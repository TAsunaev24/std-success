#include <iostream>
#include "../backend/field.h"
#include "shared.h"

using namespace std;

int main()
{
	Field field = Field(SIZE, SIZE);
	for (int i = 0; i<SIZE; ++i)
	{
		for (int j = 0; j<SIZE; ++j)
			cout << (field.getCell(i, j)==WALL?'O':' ');

		cout << endl;
	}
	return 0; 
}
