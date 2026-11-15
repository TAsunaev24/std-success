#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "field.h"
#include "utils.h"

using namespace std;

struct UnitTestEntry
{
	double (*checker)(int);
	string name;
	int arg;
	string argDescription;
	string resultExplaination;
};

class UnitTests
{
	private:
		vector<UnitTestEntry> _entries;

	public:
		UnitTests();
		UnitTests(vector<UnitTestEntry> entries);
		void addTest(double (*checker)(int), int checkerArgument, string testName, string argumentDescription, string resultExplaination);
		vector<UnitTestEntry> getEntries();
		double getPassingPercentageForTest(int index, int count);
};

UnitTests::UnitTests()
{
}

UnitTests::UnitTests(vector<UnitTestEntry> entries)
{
	_entries = entries;
}

void UnitTests::addTest(double (*checker)(int), int checkerArgument, string testName, string argumentDescription, string resultExplaination)
{
	UnitTestEntry entry = { checker, testName, checkerArgument, resultExplaination };
	_entries.push_back(entry);
}

double UnitTests::getPassingPercentageForTest(int index, int count)
{
	if (index<0 || index >= _entries.size()) return -1.0;
	double sum = 0;
	for (int i = 0; i < count; i++)
		sum += _entries[index].checker(_entries[index].arg);

	return sum / count;
}

vector<UnitTestEntry> UnitTests::getEntries()
{
	return _entries;
}

char getSignForResult(double value)
{
    if (value >= 0)
    {
        return '>';
    }
    else
    {
        return '<';
    }
}

string generateCSVTableForTest(vector<UnitTestEntry> entries, double* results, double* passingValues)
{
    stringstream csv;

    // header
    csv << "#,Description,Test Data,Expectations,Actual Result,Result Explaination,Status\n"; // 8 columns

    for (size_t i = 0; i < entries.size(); i++)
    {
        csv << i << ","; // #
        csv << entries[i].name << ","; // Description
        csv << entries[i].argDescription << ": " << entries[i].arg << ","; // Test Data
        csv << getSignForResult(passingValues[i]);
        csv << abs(passingValues[i] * 100) << "%,"; // Expected
        csv << abs(results[i] * 100) << "%,"; // Actual Result
		csv << entries[i].resultExplaination << ",";
        if (results[i] > passingValues[i])
        {
            csv << "Passed,\n";
        }
        else
        {
            csv << "Failed,\n";
        }
    }

    return csv.str();
}

double labyrinth_checker(int size)
{
	Field field = Field(size, size, false);
	if (field.getCell(size - 1, size - 1) == EMPTY)
		return 1; // true
	else
		return 0; // false
}

double labyrinth_wall_ratio_checker(int size)
{
	Field field = Field(size, size, false);
	int wallQuantity = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (field.getCell(i, j) == WALL)
			{
				wallQuantity++;
			}
		}
	}

	return -((double)wallQuantity)/(size*size);
}

double random_checker(int range)
{
	int *randomArrayCount = new int[range];

	for (int i = 0; i < range; i++)
	{
		int index = getRandom() % range;
		randomArrayCount[index]++;
	}

	int nonEmptyQuantity = 0;
	for (int i = 0; i < range; i++)
	{
		if (randomArrayCount[i] > 0)
		{
			nonEmptyQuantity++;
		}
	}

	delete[] randomArrayCount;
	return ((double)nonEmptyQuantity) / range;
}

int main()
{
	UnitTests tests = UnitTests();
	tests.addTest(labyrinth_checker, 30, "Generated labyrinth has a path to the end", "Labyrinth size", "% of total labyrinths generated with a way to get to the finish");
	tests.addTest(labyrinth_wall_ratio_checker, 30, "Generated labyrinth does not have wall congestion", "Labyrinth size", "% of wall in relation to the total cell count");
	tests.addTest(random_checker, 1000, "Randomizer function(getRandom()) returns nonrepeating values", "Range", "100% means every value generated is unique");

	cout << "Tests pending:" << endl;

	vector<UnitTestEntry> entries = tests.getEntries();
	for (size_t i = 0; i < entries.size(); i++)
	{
		cout << "#" << i + 1 << ": " << entries[i].name << " with argument " << entries[i].arg << endl;
	}

	double *results = new double[entries.size()];
	double desired[] = { 0.9, -0.5, 0.9 }; // Negative value means that the result should be lower that the desired value in order to pass
	for (int i = 0; i < entries.size(); i++)
	{
		results[i] = tests.getPassingPercentageForTest(i, 1000);
		if (results[i] >= desired[i])
		{
			cout << "Test #" << i + 1 << " PASSED" << endl;
		}
		else
		{
			cout << "Test #" << i + 1 << " FAILED" << endl;
		}
	}

	cout << "Writing results to results.csv" << endl;

	string str = generateCSVTableForTest(entries, results, desired);
	ofstream of = ofstream("results.csv");
	of << str;
	of.close();

	cout << "Done" << endl;

	return 0;
}
