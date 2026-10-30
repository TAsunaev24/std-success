#include <iostream>
#include "../backend/field.h"

using namespace std;

int main()
{
	Field field = Field(15, 15);
	for (int i = 0; i<15; ++i)
	{
		for (int j = 0; j<15; ++j)
			cout << (field.getCell(i, j)==WALL?'W':' ');

		cout << endl;
	}
	return 0; 
}
