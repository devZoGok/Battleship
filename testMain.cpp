#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace CppUnit;

int main(){
	TextUi::TestRunner runner;
	runner.run();
	return 0;
}
