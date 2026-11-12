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


