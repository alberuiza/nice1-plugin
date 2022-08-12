
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
		strcpy(res, string);
		return res;
	}

	//EXPORT_API const char* TestString(char* url, int length) {
	//	static char* dialogTitle;
	//	dialogTitle = MakeStringCopy(url);
	//	return dialogTitle;
	//}

	// return 1 -> License OK
	// return 0 -> License FAIL
	//EXPORT_API int CheckLicense(char* owner, char* author, char* category, char* license_name, char* idata_name) {

	//	static char* authorChar;
	//	static char* authorChar2;
	//	static char* license_nameChar;
	//	static char* idata_nameChar;


	//	license_nameChar = MakeStringCopy(license_name);
	//	std::string license_nameStr(license_nameChar);

	//	authorChar = MakeStringCopy(author);
	//	std::string authorStr(authorChar);

	//	idata_nameChar = MakeStringCopy(idata_name);
	//	std::string idata_nameStr(idata_nameChar);

	//	authorChar2 = MakeStringCopy(author);
	//	std::string author2Str(authorChar2);

	//	std::string str1 = authorStr + license_nameStr;
	//	std::string str2 = author2Str + idata_nameStr;

	//	if (str1.compare(str2) == 0)
	//	{
	//		return 1;
	//	}
	//	else
	//	{
	//		return 0;
	//	}
	//}

	//EXPORT_API int CheckNice1GenesisKey(char* owner, char* author, char* category, char* license_name, char* idata_name, int checkNice1Key) {
	//	// Nice1 Genesis Key
	//	static char* idata_name_genesisKey;
	//	static char* author_genesisKey;
	//	static char* category_genesisKey;
	//	static char* authorChar2;
	//	static char* idata_nameChar;

	//	idata_name_genesisKey = MakeStringCopy("NICE1 Genesis Key");
	//	std::string idata_name_genesisKeyStr(idata_name_genesisKey);
	//	author_genesisKey = MakeStringCopy("niceonechain");
	//	std::string author_genesisKeyStr(author_genesisKey);
	//	category_genesisKey = MakeStringCopy("niceoneepics");

	//	idata_nameChar = MakeStringCopy(idata_name);
	//	std::string idata_nameStr(idata_nameChar);

	//	authorChar2 = MakeStringCopy(author);
	//	std::string author2Str(authorChar2);

	//	std::string str2 = author2Str + idata_nameStr;
	//	std::string str3 = author_genesisKeyStr + idata_name_genesisKeyStr;

	//	if (checkNice1Key == 1 && (str3.compare(str2) == 0))
	//	{
	//		return 1;
	//	}
	//	else
	//	{
	//		return 0;
	//	}
	//}


	string networkEndpoints[] = {
		"https://jungle3.api.simpleassets.io/v1/", // Jungle3 Testnet
		"https://eos.api.simpleassets.io/v1/", // EOS Mainnet
		"https://telostest.eu.eosamsterdam.net/v1", // Telos Testnet
		"https://telos.api.simpleassets.io/v1/", // Telos Mainnet
		"https://test.proton.eosusa.news/v2/", // Proton Testnet
		"https://proton.greymass.com/v1/", // Proton Mainnet
		"https://testnet.waxsweden.org/v2/", // WAX Testnet
		"https://wax.eu.eosamsterdam.net/v2/" // WAX Mainnet
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
					strcat(author, name);
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

				strcat(author, name.c_str());
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
		strcpy(licenseCheck, authorChar);
		strcpy(licenseCheck + strlen(authorChar), license_nameChar);
		
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
		if (network == 0 || network == 1 || network == 3) simpleAssets = true;
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