#ifndef PATHFINDER_TEST_H
#define PATHFINDER_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace battleship{
		class Pathfinder;

		class PathfinderTest : public CppUnit::TestFixture{
				CPPUNIT_TEST_SUITE(PathfinderTest);
				CPPUNIT_TEST(testFindPath);
				CPPUNIT_TEST(testFindBigPath);
				CPPUNIT_TEST_SUITE_END();

				public:
						PathfinderTest(){}
						void testFindPath();
						void testFindBigPath();
						void setUp();
						void tearDown();
				private:
						Pathfinder *pathfinder = nullptr;
		};
}

#endif
