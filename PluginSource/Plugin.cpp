
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

using namespace std;

extern "C"
{
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
	{
		((std::string*)userp)->append((char*)contents, size * nmemb);
		return size * nmemb;
	}

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
		"http://Jungle4.cryptolions.io", // Jungle4 Testnet
		"https://eos.greymass.com", // EOS Mainnet
		"https://test.telos.eosusa.io/v2/", // Telos Testnet
		"https://telos.greymass.com", // Telos Mainnet
		"https://test.proton.eosusa.io/v2/", // Proton Testnet
		"https://proton.cryptolions.io/v2/", // Proton Mainnet
		"https://test.wax.eosusa.io", // WAX Testnet
		"https://wax.greymass.com" // WAX Mainnet
	};

	bool simpleAssets;

	const char* CURL_INIT_ERROR_MESSAGE = "CURL_INIT_ERROR";
	const char* CURL_REQUEST_ERROR_MESSAGE = "CURL_REQUEST_ERROR";

	const char* GetUrl(const char* ownerChar, const char* authorChar, const char* categoryChar, int network)
	{
		string baseUrl = networkEndpoints[network];

		if (simpleAssets)
			baseUrl = baseUrl + "assets/search?author=" + authorChar + "&owner=" + ownerChar + "&category=" + categoryChar + "&page=1&limit=1000&sortField=assetId&sortOrder=asc";
		else
			baseUrl = baseUrl + "history/get_deltas?code=simpleassets&scope=" + ownerChar;

		return MakeStringCopy(baseUrl.c_str());
	}

	const char* GetCurlResponse(const char* url)
	{
		CURL* curl;
		CURLcode response;
		string readBuffer;

		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();

		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
			response = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			curl_global_cleanup();

			if (response == 0) {
				return MakeStringCopy(readBuffer.c_str());

			}
			else {
				return MakeStringCopy(CURL_REQUEST_ERROR_MESSAGE + response);
			}
		}
		else {
			return MakeStringCopy(CURL_INIT_ERROR_MESSAGE);
		}
	}

	bool IsValid(JsonNode* node)
	{
		return node != nullptr && node;
	}

	const char* GetAuthorNameSimpleAssets(JsonNode* element)
	{
		char* author = nullptr;
		JsonNode* authorMember = json_find_member(element, "author");
		if (IsValid(authorMember) && authorMember->tag == JSON_STRING) {
			author = authorMember->string_;
		}

		JsonNode* tmp;
		json_foreach(tmp, element)
		{
			if (IsValid(tmp) && tmp->tag == JSON_OBJECT)
			{
				JsonNode* nameMember = json_find_member(tmp, "name");
				if (IsValid(nameMember) && nameMember->tag == JSON_STRING) {
					char* name = nameMember->string_;
					//strcat(author, name);
					strcat_s(author, strlen(name)+1, name);
					break;
				}
			}
		}

		free(tmp);
		return author;
	}

	const char* GetAuthorNameDeltas(JsonNode* element)
	{
		JsonNode* data = json_find_member(element, "data");

		if (IsValid(data) && data->tag == JSON_OBJECT)
		{
			char* author = nullptr;
			JsonNode* authorMember = json_find_member(data, "author");
			if (IsValid(authorMember) && authorMember->tag == JSON_STRING) {
				author = authorMember->string_;
			}

			char* idata = nullptr;
			JsonNode* idataMember = json_find_member(data, "idata");
			if (IsValid(idataMember) && idataMember->tag == JSON_STRING) {
				idata = idataMember->string_;

				string name = idata; // {"name":"GAME LICENSE - LegendaryLegends"} 
				int index = name.find(":");
				name = name.substr(index + 1, name.length() - index - 1); //"GAME LICENSE - LegendaryLegends"} 
				index = name.find("\"");
				name = name.substr(index + 1, name.length() - index - 1); //GAME LICENSE - LegendaryLegends"} 
				index = name.find("\"");
				name = name.substr(0, index); //GAME LICENSE - LegendaryLegends

				//strcat(author, name.c_str());
				strcat_s(author, strlen(name.c_str()) + 1, name.c_str());
			}
			return author;
		}
		return "";
	}

	const char* GetLicenseFromJson(const char* curlResponse, const char* licenseCheck)
	{
		JsonNode* tmpJSon = json_decode(curlResponse);
		JsonNode* results;
		if (simpleAssets) results = json_find_member(tmpJSon, "results");
		else results = json_find_member(tmpJSon, "deltas");

		if (results != nullptr)
		{
			if (results && results->tag == JSON_ARRAY)
			{
				bool license = false;
				JsonNode* element;
				json_foreach(element, results)
				{
					if (IsValid(element) && element->tag == JSON_OBJECT)
					{
						char* authorNameChar = nullptr;
						if (simpleAssets) authorNameChar = MakeStringCopy(GetAuthorNameSimpleAssets(element));
						else authorNameChar = MakeStringCopy(GetAuthorNameDeltas(element));

						int compare = strcmp(authorNameChar, licenseCheck);
						if (compare == 0) {
							license = true;
							break;
						}
						else {
							license = false;
						}
					}
					else {
						break;
					}
				}

				free(results);

				if (license) {
					return "LICENSE";
				}
				else {
					return "ERROR LICENSE";
				}
			}
			else {
				return "ERROR-NO-ARRAY";
			}
		}
		else {
			return "ERROR-NODE-NULL";
		}
	}

	const char* CheckLicense(char* owner, char* author, char* category, char* license_name, int network)
	{
		const char* authorChar = MakeStringCopy(author);
		const char* ownerChar = MakeStringCopy(owner);
		const char* categoryChar = MakeStringCopy(category);
		const char* license_nameChar = MakeStringCopy(license_name);

		const char* url = GetUrl(ownerChar, authorChar, categoryChar, network);
		
		const char* curlResponse = GetCurlResponse(url);
		if (strcmp(curlResponse, CURL_INIT_ERROR_MESSAGE) == 0) return CURL_INIT_ERROR_MESSAGE;
		if (strcmp(curlResponse, CURL_REQUEST_ERROR_MESSAGE) == 0) return CURL_REQUEST_ERROR_MESSAGE;
		
		char* licenseCheck = (char*)malloc(strlen(authorChar) + strlen(license_nameChar) + 1);
		//strcpy(licenseCheck, authorChar);
		strcpy_s(licenseCheck, strlen(authorChar)+strlen(license_nameChar) +1, authorChar);
		//strcpy(licenseCheck + strlen(authorChar), license_nameChar);
		strcpy_s(licenseCheck + strlen(authorChar), strlen(authorChar) + strlen(license_nameChar) + strlen(authorChar) + 1,  license_nameChar);
		
		return GetLicenseFromJson(curlResponse, licenseCheck);
	}

	const char* CheckNice1GenesisKey(char* owner, int network)
	{
		static char* author_genesisKey = MakeStringCopy("niceonechain");
		static char* category_genesisKey = MakeStringCopy("niceoneepics");
		static char* idata_name_genesisKey = MakeStringCopy("NICE1 Genesis Key");

		return CheckLicense(owner, author_genesisKey, category_genesisKey, idata_name_genesisKey, network);
	}

	EXPORT_API const char* CheckLicense2TEST(char* owner, char* author, char* category, char* license_name, int checkNice1GenesisKey, int network)
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