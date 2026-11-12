#include <fstream>
#include "../backend/field.h"
#include "../backend/utils.h"
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct UnitTestEntry
{
	double (*checker)(int);
	string name;
	int arg;
};

class UnitTests
{
	private:
		vector<UnitTestEntry> _entries;

	public:
		UnitTests();
		UnitTests(vector<UnitTestEntry> entries);
		void addTest(double (*checker)(int), int checkerArgument, string testName);
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

void UnitTests::addTest(double (*checker)(int), int checkerArgument, string testName)
{
	UnitTestEntry entry = { checker, testName, checkerArgument};
	_entries.push_back(entry);
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

string generateCSVTableForTest(vector<UnitTestEntry> entries, double results, doublepassingValues)
{
    stringstream csv;

    // header
    csv << "#,Description,Test Data,Expectations,Actual Result,Status\n"; // 8 columns

    for (size_t i = 0; i < entries.size(); i++)
    {
        csv << i << ","; // #
        csv << entries[i].name << ","; // Description
        csv << entries[i].arg << ","; // Test Data
        csv << getSignForResult(passingValues[i]);
        csv << abs(passingValues[i] * 100) << "%,"; // Expected
        csv << abs(results[i] * 100) << "%,"; // Actual Result
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
int main()
{
	UnitTests tests = UnitTests();

	cout << "Tests pending:" << endl;

	vector<UnitTestEntry> entries = tests.getEntries();
	for (size_t i = 0; i < entries.size(); i++)
	{
		cout << "#" << i + 1 << ": " << entries[i].name << " with argument " << entries[i].arg << endl;
	}

	double *results = new double[entries.size()];
	double desired[]; // TODO add desired values;	
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
