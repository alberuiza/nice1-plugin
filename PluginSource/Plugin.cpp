
#if _MSC_VER 
#define EXPORT_API __declspec(dllexport) 
#else
#define EXPORT_API 
#endif

// ------------------------------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <curl/curl.h>
#include "json.h"
#include "json.c"
#include <nlohmann/json.hpp>
#include "JsonDataContainer.h"

using namespace quicktype;
using namespace std;
using json = nlohmann::json;

extern "C"
{
	// This function is used as a callback for the libcurl library and
	// is responsible for receiving data received by the HTTP request
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
	{
		((std::string*)userp)->append((char*)contents, size * nmemb);
		return size * nmemb;
	}

	// This function takes a character string and creates a dynamic copy
	// of it in memory, returning a pointer to the new block of memory
	char* MakeStringCopy(const char* string) {
		if (string == NULL) return NULL;
		char* res = (char*)malloc(strlen(string) + 1);
		//strcpy(res, string);
		if (res != NULL)
		{
			strcpy_s(res, strlen(string) + 1, string);
		}
		return res;
	}

	string networkEndpoints[] = {
		"http://jungle4.greymass.com/v1/chain/get_table_rows", // Jungle4 Testnet
		"https://eos.greymass.com/v1/chain/get_table_rows", // EOS Mainnet
		"https://protontestnet.greymass.com/v1/chain/get_table_rows", // Proton Testnet
		"https://proton.cryptolions.io/v1/chain/get_table_rows", // Proton Mainnet
		"https://testnet.waxsweden.org/v1/chain/get_table_rows", // WAX Testnet
		"https://wax.greymass.com/v1/chain/get_table_rows", // WAX Mainnet
		"https://test.telos.eosusa.io/v1/chain/get_table_rows", // TELOS Testnet
		"https://telos.greymass.com/v1/chain/get_table_rows" // TELOS Mainnet
	};

	bool simpleAssets;

	const char* CURL_INIT_ERROR_MESSAGE = "CURL_INIT_ERROR";
	const char* CURL_REQUEST_ERROR_MESSAGE = "CURL_REQUEST_ERROR";

	const char* GetCurlResponse(const char* url, char* owner)
	{
		CURL* curl;
		CURLcode response;
		unique_ptr<string> httpData(new string());

		string own = owner;

		string jsonstr = "{\r\n    \"json\": true,\r\n    \"code\": \"simpleassets\",\r\n    \"scope\": \"" + own + "\",\r\n    \"table\": \"sassets\",\r\n    \"index position\": \"secondary\",\r\n    \"key type\": \"name\",\r\n    \"lower bound\": \"niceonedemos\",\r\n    \"upper bound\": \"niceonedemos\",\r\n    \"limit\": 100,\r\n    \"reverse\": false,\r\n    \"show_payer\": false\r\n}";

		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();

		if (curl) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

			struct curl_slist* headers = NULL;
			headers = curl_slist_append(headers, "Content-Type: application/json");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr.c_str());

			response = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			curl_global_cleanup();

			if (response == 0) {
				return MakeStringCopy(httpData.get()->c_str());
			}
			else {
				return MakeStringCopy(CURL_REQUEST_ERROR_MESSAGE + response);
			}
		}
		else {
			return MakeStringCopy(CURL_INIT_ERROR_MESSAGE);
		}
	}

	const char* GetLicenseFromJsonData(const char* curlResponse, const char* licenseCheck, char* category)
	{
		JsonDataContainer jsonData = json::parse(curlResponse);

		// If we do not have data
		if (jsonData.get_rows().size() == 0)
			return "ERROR-NO-NFTS";

		// Indicates if it has the license
		bool license = false;

		// We filter each row of the JSON
		for (Row row : jsonData.get_rows())
		{
			// Comparing the category
			if (strcmp(row.get_category().c_str(), category) == 0)
			{
				// If the category matches, we compare it with the license
				string licenseData = row.get_author() + row.get_idata().get_name();
				if (strcmp(licenseData.c_str(), licenseCheck) == 0) {
					license = true;
					break;
				}
				else
					license = false;
			}
		}

		// If it has a license
		if (license)
			return "LICENSE";
		// Otherwise
		else
			return "ERROR LICENSE";

	}

	const char* CheckLicense(char* owner, char* author, char* category, char* license_name, int network)
	{
		const char* authorChar = MakeStringCopy(author);
		const char* ownerChar = MakeStringCopy(owner);
		const char* categoryChar = MakeStringCopy(category);
		const char* license_nameChar = MakeStringCopy(license_name);

		const char* url = MakeStringCopy(networkEndpoints[network].c_str());

		const char* curlResponse = GetCurlResponse(url, owner);
		if (strcmp(curlResponse, CURL_INIT_ERROR_MESSAGE) == 0) return CURL_INIT_ERROR_MESSAGE;
		if (strcmp(curlResponse, CURL_REQUEST_ERROR_MESSAGE) == 0) return CURL_REQUEST_ERROR_MESSAGE;

		string authorC = author;
		string license_nameC = license_name;
		string licenseCheck = authorC + license_nameC;

		return GetLicenseFromJsonData(curlResponse, licenseCheck.c_str(), category);
	}

	const char* CheckNice1GenesisKey(char* owner, int network)
	{
		static char* author_genesisKey = MakeStringCopy("niceonedemos");
		static char* category_genesisKey = MakeStringCopy("niceoneepics");
		static char* idata_name_genesisKey = MakeStringCopy("NICE1 Genesis Key");

		return CheckLicense(owner, author_genesisKey, category_genesisKey, idata_name_genesisKey, network);
	}

	EXPORT_API const char* CheckLicensePlugin(char* owner, char* author, char* category, char* license_name, int checkNice1GenesisKey, int network)
	{
		if (network == 0 || network == 1 || network == 3 || network == 7) simpleAssets = true;
		else simpleAssets = false;
		
		if (checkNice1GenesisKey == 1)
		{
			const char* response = CheckNice1GenesisKey(owner, network);

			if (strcmp(response, "LICENSE") == 0)
			{
				return "NICE1KEY";
			}
			else {
				return CheckLicense(owner, author, category, license_name, network);
			}
		}
		else
		{
			return CheckLicense(owner, author, category, license_name, network);
		}
	}

}