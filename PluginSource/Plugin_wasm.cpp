
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
#include <emscripten/fetch.h>
#include "json.h"
#include "json.c"
#include "json.hpp"
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
			strcpy(res, string);
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
	
	char* globalResponse = nullptr;
	
	 
    char* globalAuthor = nullptr;
    char* globalCategory = nullptr;
    char* globalLicenseName = nullptr;
	
	int globalLicenseCheckState = 0; // 0 for not started, 1 for in progress, 2 for completed

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
	
void ProcessLicenseCheckResponse() {
		// This function should be called from the downloadSucceeded callback
		if (globalResponse != nullptr) {
			string authorC = globalAuthor;
			string license_nameC = globalLicenseName;
			string licenseCheck = authorC + license_nameC;

			const char* result = GetLicenseFromJsonData(globalResponse, licenseCheck.c_str(), globalCategory);

			// Use result as needed

			// Clean up
			delete[] globalResponse;
			globalResponse = nullptr;
			free(globalAuthor);
			free(globalCategory);
			free(globalLicenseName);
		}
	}

void downloadSucceeded(emscripten_fetch_t *fetch) {
    // Copy the data into a newly allocated buffer
    globalResponse = new char[fetch->numBytes + 1];
    memcpy(globalResponse, fetch->data, fetch->numBytes);
    globalResponse[fetch->numBytes] = '\0'; // Null-terminate the string
	ProcessLicenseCheckResponse();
    emscripten_fetch_close(fetch); // Free up memory used by the fetch
}

void downloadFailed(emscripten_fetch_t *fetch) {
    // Handle failed download, possibly setting globalResponse to a relevant error message
    globalResponse = strdup("Download failed");

    emscripten_fetch_close(fetch); // Free up memory used by the fetch
}

	void GetCurlResponse(const char* url, char* owner)
	{
		 emscripten_fetch_attr_t attr;
		emscripten_fetch_attr_init(&attr);
		strcpy(attr.requestMethod, "POST");

		// Set up headers
		const char* headers[] = {"Content-Type", "application/json", nullptr};
		attr.requestHeaders = headers;

		// Prepare the JSON payload
		std::string own = owner;
		std::string jsonstr = "{\n    \"json\": true,\n    \"code\": \"simpleassets\",\n    \"scope\": \"" + own + "\",\n    \"table\": \"sassets\",\n    \"index position\": \"secondary\",\n    \"key type\": \"name\",\n    \"lower bound\": \"niceonedemos\",\n    \"upper bound\": \"niceonedemos\",\n    \"limit\": 100,\n    \"reverse\": false,\n    \"show_payer\": false\n}";
		attr.requestData = jsonstr.c_str();
		attr.requestDataSize = jsonstr.length();

		// Set the callback functions
		attr.onsuccess = downloadSucceeded;
		attr.onerror = downloadFailed;

		// Perform the fetch
		emscripten_fetch(&attr, url);
	}

	

	void CheckLicense(char* owner, char* author, char* category, char* license_name, int network)
	{
		// Copy necessary data
		char* url = MakeStringCopy(networkEndpoints[network].c_str());
		globalAuthor = MakeStringCopy(author);
		globalCategory = MakeStringCopy(category);
		globalLicenseName = MakeStringCopy(license_name);

		// Initiate the request
		GetCurlResponse(url, owner);
	}
	
	

	const void CheckNice1GenesisKey(char* owner, int network)
	{
		static char* author_genesisKey = MakeStringCopy("niceonedemos");
		static char* category_genesisKey = MakeStringCopy("niceoneepics");
		static char* idata_name_genesisKey = MakeStringCopy("NICE1 Genesis Key");

		 CheckLicense(owner, author_genesisKey, category_genesisKey, idata_name_genesisKey, network);
	}

	EXPORT_API void CheckLicensePlugin(char* owner, char* author, char* category, char* license_name, int checkNice1GenesisKey, int network) {
		if (network == 0 || network == 1 || network == 3 || network == 7) simpleAssets = true;
		else simpleAssets = false;
		
		globalLicenseCheckState = 1; // Mark the license check as in progress

		if (checkNice1GenesisKey == 1) {
			CheckNice1GenesisKey(owner, network); // Initiates the license check
		} else {
			CheckLicense(owner, author, category, license_name, network); // Initiates the license check
		}
	}
	
		// Call this function to check if the license check is completed and to get the result
	EXPORT_API const char* GetLicenseCheckResult() {
		if (globalLicenseCheckState == 2) {
			// The result is ready
			globalLicenseCheckState = 0; // Reset the state
			return globalResponse; // globalResponse holds the result
		}
		return nullptr; // Indicates that the result is not ready yet
	}

}