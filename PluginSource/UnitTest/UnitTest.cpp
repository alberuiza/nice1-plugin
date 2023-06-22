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

		// Test method to test WriteCallback
		TEST_METHOD(TestWriteCallback)
		{
			std::string str = "Test";
			std::string output;
			WriteCallback((void*)str.c_str(), 1, str.length(), (void*)&output);
			Assert::AreEqual(str, output);
		}

		// Test method to test GetUrl
		TEST_METHOD(TestGetUrl)
		{
			const char* ownerChar = "test";
			const char* authorChar = "niceonetest1";
			const char* categoryChar = "test";
			int network = 0;
			const char* output = GetUrl(ownerChar, authorChar, categoryChar, network);
			Assert::AreEqual("http://jungle4.greymass.com/v1/chain/get_account", output);
			network = 1;
			output = GetUrl(ownerChar, authorChar, categoryChar, network);
			Assert::AreEqual("http://jungle4.greymass.com/history/get_deltas?code=simpleassets&scope=test", output);
			// + networkEndpoints[]
		}

		// Test method to test GetCurlResponse
		TEST_METHOD(TestGetCurlResponse)
		{
			const char* url = "https://jungle3.api.simpleassets.io/v1/assets/search?author=test&owner=test&category=test&page=1&limit=1000&sortField=assetId&sortOrder=asc";
			const char* output = GetCurlResponse(url);
			Assert::AreEqual("{\"results\":[],\"extra\":{\"total\":0}}", output);
		}

		// Test method to test GetAuthorNameSimpleAssets
	/*	TEST_METHOD(TestGetAuthorNameSimpleAssets)
		{
			JsonNode* node;
			const char* output = GetAuthorNameSimpleAssets(node);
			Assert::AreEqual("nice1_name_account", output);
		}*/
		
		// Test method to test GetAuthorNameDeltas
	/*	TEST_METHOD(TestGetAuthorNameDeltas)
		{
			JsonNode* node;
			const char* output = GetAuthorNameDeltas(node);
			Assert::AreEqual("nice1_author_account", output);
		}*/
		
		// Test method to test GetLicenseFromJson
	/*	TEST_METHOD(TestGetLicenseFromJson)
		{
			const char* curlResponse;
			const char* licenseCheck;
			const char* output = GetLicenseFromJson(curlResponse,licenseCheck);
			Assert::AreEqual("LICENSE", output);
		}*/

		// Test method to test CheckLicense
		TEST_METHOD(TestCheckLicense)
		{
			char* ownerChar = "test";
			char* authorChar = "test";
			char* categoryChar = "test";
			char* license_nameChar = "test";
			int network = 0;
			const char* output = CheckLicense(ownerChar, authorChar, categoryChar, license_nameChar, network);
			Assert::AreEqual("ERROR-NODE-NULL", output);

			// Error message: "ERROR LICENSE"
			/*ownerChar = "test";
			authorChar = "test";
			categoryChar = "test";
			license_nameChar = "test";
			network = 0;
			output = CheckLicense(ownerChar, authorChar, categoryChar, license_nameChar, network);
			Assert::AreEqual("ERROR LICENSE", output);*/
		}

		// Test method to test CheckNice1GenesisKey
		TEST_METHOD(TestCheckNice1GenesisKey)
		{
			char* ownerChar = "test";
			int network = 0;
			const char* output = CheckNice1GenesisKey(ownerChar, network);
			Assert::AreEqual("ERROR-NODE-NULL", output);

			// Message: "LICENSE"
			/*ownerChar = "test";
			network = 0;
			output = CheckNice1GenesisKey(ownerChar, network);
			Assert::AreEqual("LICENSE", output);*/
		}
		
		// Test method to test CheckLicense2TEST
		TEST_METHOD(TestCheckLicense2TEST)
		{
			char* ownerChar = "test";
			char* authorChar = "test";
			char* categoryChar = "test";
			char* license_nameChar = "test";
			int network = 0;
			int checkNice1GenesisKey = 0;
			const char* output = CheckLicense2TEST(ownerChar, authorChar, categoryChar, license_nameChar, checkNice1GenesisKey,  network);
			Assert::AreEqual("ERROR LICENSE", output);

			// Message "NICE1KEY"
			/*ownerChar = "test";
			authorChar = "test";
			categoryChar = "test";
			license_nameChar = "test";
			network = 0;
			checkNice1GenesisKey = 0;
			output = CheckLicense2TEST(ownerChar, authorChar, categoryChar, license_nameChar, checkNice1GenesisKey, network);
			Assert::AreEqual("NICE1KEY", output);*/
		}
		
		
	};
}
