#include "pch.h"
#include "CppUnitTest.h"
#include "../Plugin.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(TestMakeStringCopy)
		{
			const char* str = "Test";
			char *output = MakeStringCopy(str);
			Assert::AreEqual(str, output);
		}
	};
}
