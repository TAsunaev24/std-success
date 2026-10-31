#include <iostream>
#include "../backend/field.h"

using namespace std;

#define COUNT 50000
#define SIZE 25

int main()
{
	int countGood = 0;
	for (int i = 0; i<COUNT; ++i)
	{
		Field *field = new Field(SIZE, SIZE);
		if (field->getCell(SIZE-1, SIZE-1)==EMPTY) ++countGood;
		delete field;
	}
	cout << "The test passed " << ((double)countGood)/COUNT*100 << "% of the times" << endl;
}
