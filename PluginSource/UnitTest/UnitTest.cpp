#include "pch.h"
#include "CppUnitTest.h"
#include "../Plugin.h"
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
			char* output = MakeStringCopy(str);
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

		// Test method to test GetCurlResponse
		TEST_METHOD(TestGetCurlResponse)
		{
			// With Jungle3
			const char* url = "https://jungle3.api.simpleassets.io/v1/assets/search?author=test&owner=test&category=test&page=1&limit=1000&sortField=assetId&sortOrder=asc";
			const char* output = GetCurlResponse(url, "testnice1");
			Assert::AreEqual("{\"results\":[],\"extra\":{\"total\":0}}", output);

			// With Jungle4 Testnet
			url = "http://jungle4.greymass.com/v1/chain/get_table_rows";
			output = GetCurlResponse(url, "testnice1");
			Assert::AreEqual("{\"rows\":[],\"more\":false,\"next_key\":\"\"}", output);

			// With EOS Mainnet
			url = "https://eos.greymass.com/v1/chain/get_table_rows";
			output = GetCurlResponse(url, "testnice1");
			Assert::AreEqual("{\"rows\":[],\"more\":false,\"next_key\":\"\"}", output);

			// With Proton Testnet
			url = "https://eos.greymass.com/v1/chain/get_table_rows";
			output = GetCurlResponse(url, "testnice1");
			Assert::AreEqual("{\"rows\":[],\"more\":false,\"next_key\":\"\"}", output);

			// With Proton Mainnet
			url = "https://proton.cryptolions.io/v1/chain/get_table_rows";
			output = GetCurlResponse(url, "testnice1");
			Assert::AreEqual("{\"rows\":[],\"more\":false,\"next_key\":\"\"}", output);

			// With WAX Testnet
			url = "https://testnet.waxsweden.org/v1/chain/get_table_rows";
			output = GetCurlResponse(url, "testnice1");
			Assert::AreEqual("{\"rows\":[],\"more\":false,\"next_key\":\"\"}", output);

			// With WAX Mainnet
			url = "https://wax.greymass.com/v1/chain/get_table_rows";
			output = GetCurlResponse(url, "testnice1");
			Assert::AreEqual("{\"rows\":[],\"more\":false,\"next_key\":\"\"}", output);

			//// With TELOS Testnet
			//url = "https://test.telos.eosusa.io/v1/chain/get_table_rows";
			//output = GetCurlResponse(url, "test");
			//Assert::AreEqual("{\"rows\":[],\"more\":false,\"next_key\":\"\"}", output);

			//// With TELOS Mainnet
			//url = "https://telos.greymass.com/v1/chain/get_table_rows";
			//output = GetCurlResponse(url, "test");
			//Assert::AreEqual("{\"rows\":[],\"more\":false,\"next_key\":\"\"}", output);

		}

		// Test method to test CheckLicense
		TEST_METHOD(TestCheckLicense)
		{
			char* ownerChar = "testnice1";
			char* authorChar = "testnice1";
			char* categoryChar = "testnice1";
			char* license_nameChar = "testnice1";
			// Error message: "ERROR-NO-NFTS"
			for (int network = 0; network < 6; network += 2)
			{
				const char* output = CheckLicense(ownerChar, authorChar, categoryChar, license_nameChar, network);
				Assert::AreEqual("ERROR-NO-NFTS", output);
			}

			// Error message: "ERROR LICENSE"
			authorChar = "niceonedemos";
			categoryChar = "llegends";
			license_nameChar = "testnice1";
			for (int network = 0; network < 6; network += 2)
			{
				if (network == 0)
					ownerChar = "niceonetest1";
				else
					ownerChar = "niceonedemos";

				const char* output = CheckLicense(ownerChar, authorChar, categoryChar, license_nameChar, network);
				Assert::AreEqual("ERROR LICENSE", output);
			}

			// Message: "LICENSE"
			authorChar = "niceonedemos";
			categoryChar = "llegends";
			license_nameChar = "GameKey";
			for (int network = 0; network < 6; network += 2)
			{
				if (network == 0)
					ownerChar = "niceonetest1";
				else
					ownerChar = "niceonedemos";

				const char* output = CheckLicense(ownerChar, authorChar, categoryChar, license_nameChar, network);
				Assert::AreEqual("LICENSE", output);
			}
		}

		// Test method to test CheckNice1GenesisKey
		TEST_METHOD(TestCheckNice1GenesisKey)
		{
			// Message "ERROR-NO-NFTS"
			char* ownerChar = "testnice1";

			for (int network = 0; network < 6; network += 2)
			{
				const char* output = CheckNice1GenesisKey(ownerChar, network);
				Assert::AreEqual("ERROR-NO-NFTS", output);
			}

			// Message: "LICENSE"
			for (int network = 0; network < 6; network += 2)
			{
				if (network == 0)
					ownerChar = "niceonetest1";
				else
					ownerChar = "niceonedemos";

				const char* output = CheckNice1GenesisKey(ownerChar, network);
				Assert::AreEqual("LICENSE", output);
			}

		}

		// Test method to test CheckLicensePlugin
		TEST_METHOD(TestCheckLicensePlugin)
		{
			// Message: "ERROR-NO-NFTS"
			char* ownerChar = "testnice1";
			char* authorChar = "testnice1";
			char* categoryChar = "testnice1";
			char* license_nameChar = "testnice1";
			int checkNice1GenesisKey = 0;

			for (int network = 0; network < 6; network += 2)
			{
				const char* output = CheckLicensePlugin(ownerChar, authorChar, categoryChar, license_nameChar, checkNice1GenesisKey, network);
				Assert::AreEqual("ERROR-NO-NFTS", output);
			}

			// Message: "ERROR LICENSE"
			authorChar = "niceonedemos";
			categoryChar = "llegends";
			license_nameChar = "testnice1";
			checkNice1GenesisKey = 0;

			for (int network = 0; network < 6; network += 2)
			{
				if (network == 0)
					ownerChar = "niceonetest1";
				else
					ownerChar = "niceonedemos";

				const char* output = CheckLicensePlugin(ownerChar, authorChar, categoryChar, license_nameChar, checkNice1GenesisKey, network);
				Assert::AreEqual("ERROR LICENSE", output);
			}

			// Message "LICENSE"
			authorChar = "niceonedemos";
			categoryChar = "llegends";
			license_nameChar = "GameKey";
			checkNice1GenesisKey = 0;

			for (int network = 0; network < 6; network += 2)
			{
				if (network == 0)
					ownerChar = "niceonetest1";
				else
					ownerChar = "niceonedemos";

				const char* output = CheckLicensePlugin(ownerChar, authorChar, categoryChar, license_nameChar, checkNice1GenesisKey, network);
				Assert::AreEqual("LICENSE", output);
			}

			// Message "NICE1KEY"
			authorChar = "niceonedemos";
			categoryChar = "llegends";
			license_nameChar = "GameKey";
			checkNice1GenesisKey = 1;

			for (int network = 0; network < 6; network += 2)
			{
				if (network == 0)
					ownerChar = "niceonetest1";
				else
					ownerChar = "niceonedemos";

				const char* output = CheckLicensePlugin(ownerChar, authorChar, categoryChar, license_nameChar, checkNice1GenesisKey, network);
				Assert::AreEqual("NICE1KEY", output);
			}

		}


	};
}
