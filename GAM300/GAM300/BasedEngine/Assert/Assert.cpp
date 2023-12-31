#include "Assert.h"
#include <fstream>


void debugInfo(const char* file, int line, std::string expression)
{
	// save error msg into txt file
	std::ofstream file1("../bin/AssertFailureLog.txt");
	file1 << "Assertion Failed Results:";
	file1 << "\nFailed expression: \t[" << expression.c_str() << "]";
	file1 << "\nFile Location: \t\t[" << file << "]";
	file1 << "\nLine number: \t\t[" << line << "]";
	file1.close();

	// print out error
	std::ifstream file2("../bin/AssertFailureLog.txt");
	if (file2.is_open())
	{
		file2.close();
	}

	// abort application
	abort();
}
