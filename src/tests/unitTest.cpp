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
