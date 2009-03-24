#include <gtest/gtest.h>
#include <windows.h>
#include <tchar.h>
#include <vector>
#include "stringdiffs.h"


using std::vector;

namespace
{
	// The fixture for testing String differencing functions.
	// These tests are from actually reported bugs.
	class StringDiffsBugsTest : public testing::Test
	{
	protected:
		// You can remove any or all of the following functions if its body
		// is	empty.

		StringDiffsBugsTest()
		{
			// You can do set-up work for each test	here.
			sd_Init();
		}

		virtual ~StringDiffsBugsTest()
		{
			// You can do clean-up work	that doesn't throw exceptions here.
			sd_Close();
		}

		// If	the	constructor	and	destructor are not enough for setting up
		// and cleaning up each test, you can define the following methods:

		virtual void SetUp()
		{
			// Code	here will be called	immediately	after the constructor (right
			// before each test).
		}

		virtual void TearDown()
		{
			// Code	here will be called	immediately	after each test	(right
			// before the destructor).
		}

		// Objects declared here can be used by all tests in the test case for Foo.
	};

	// sd_ComputeWordDiffs() parameters are:
	// String & str1 - the first string to compare
	// String & str2 - the second string to compare
	// bool case_sensitive - is the compare case-sensitive?
	// int whitespace - do we ignore whitespace and how
	// int breakType - Space (0) or punctuations (1) break
	// bool byte_level - are we word (false) or byte-level (true) diffing
	// std::vector<wdiff*> * pDiffs - resultting diff list

	// Sf.net bug #2638045 - word diff
	// Char added to end
	TEST_F(StringDiffsBugsTest, Bug_2638045)
	{
		std::vector<wdiff*> diffs;
		sd_ComputeWordDiffs("LIB_PHP4_DIR=$(RPM_NAME)-$(RPM_VER)/usr/lib/php",
				"LIB_PHP4_DIR=$(RPM_NAME)-$(RPM_VER)/usr/lib/php4",
				false, 0, 0, false, &diffs);
		EXPECT_EQ(1, diffs.size());
		wdiff *pDiff = diffs[0];
		if (diffs.size() == 1)
		{
			EXPECT_EQ(0, pDiff->start[0]);
			EXPECT_EQ(0, pDiff->start[1]);
			EXPECT_EQ(46, pDiff->end[0]);
			EXPECT_EQ(47, pDiff->end[1]);
		}
	}

	// Sf.net bug #2638045 - word diff with puncts
	// Char added to end
	TEST_F(StringDiffsBugsTest, Bug_2638045_2)
	{
		std::vector<wdiff*> diffs;
		sd_ComputeWordDiffs("LIB_PHP4_DIR=$(RPM_NAME)-$(RPM_VER)/usr/lib/php",
				"LIB_PHP4_DIR=$(RPM_NAME)-$(RPM_VER)/usr/lib/php4",
				false, 0, 1, false, &diffs);
		EXPECT_EQ(1, diffs.size());
		wdiff *pDiff = diffs[0];
		if (diffs.size() == 1)
		{
			EXPECT_EQ(0, pDiff->start[0]);
			EXPECT_EQ(0, pDiff->start[1]);
			EXPECT_EQ(46, pDiff->end[0]);
			EXPECT_EQ(47, pDiff->end[1]);
		}
	}

	// Sf.net bug #2638045 - char diff
	// Char added to end
	TEST_F(StringDiffsBugsTest, Bug_2638045_3)
	{
		std::vector<wdiff*> diffs;
		sd_ComputeWordDiffs("LIB_PHP4_DIR=$(RPM_NAME)-$(RPM_VER)/usr/lib/php",
				"LIB_PHP4_DIR=$(RPM_NAME)-$(RPM_VER)/usr/lib/php4",
				false, 0, 0, true, &diffs);
		EXPECT_EQ(1, diffs.size());
		wdiff *pDiff = diffs[0];
		if (diffs.size() == 1)
		{
			EXPECT_EQ(0, pDiff->start[0]);
			EXPECT_EQ(-1, pDiff->start[1]);
			EXPECT_EQ(46, pDiff->end[0]);
			EXPECT_EQ(47, pDiff->end[1]);
		}
	}

	// Sf.net bug #2638045 - custom punct word diff
	// Char added to end
	TEST_F(StringDiffsBugsTest, Bug_2638045_4)
	{
		std::vector<wdiff*> diffs;
		sd_SetBreakChars("/");
		sd_ComputeWordDiffs("LIB_PHP4_DIR=$(RPM_NAME)-$(RPM_VER)/usr/lib/php",
				"LIB_PHP4_DIR=$(RPM_NAME)-$(RPM_VER)/usr/lib/php4",
				false, 0, 1, false, &diffs);
		EXPECT_EQ(1, diffs.size());
		wdiff *pDiff = diffs[0];
		if (diffs.size() == 1)
		{
			EXPECT_EQ(44, pDiff->start[0]);
			EXPECT_EQ(44, pDiff->start[1]);
			EXPECT_EQ(46, pDiff->end[0]);
			EXPECT_EQ(47, pDiff->end[1]);
		}
	}

}  // namespace