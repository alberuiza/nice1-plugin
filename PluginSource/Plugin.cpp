
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

	string AddBarBeforeDoubleQuote(string s) {
		string other = "\\\"";

		string::size_type pos = 0;

		while (s.find("\"", pos) != std::string::npos) {
			string::size_type found = s.find("\"", pos);
			s.replace(found, 1, other);
			pos = found + 2;
		}

		return s;
	}

	string networkEndpoints[] = {
		// "http://jungle4.greymass.com", // Jungle4 Testnet (commented)
		"http://jungle4.greymass.com/v1/chain/get_table_rows", // Jungle4 Testnet
		"https://eos.greymass.com/v1/chain/get_table_rows", // EOS Mainnet
		//"https://test.telos.eosusa.io/v2/", // Telos Testnet (commented)
		"https://telostest.eu.eosamsterdam.net/v1/chain/get_table_rows", // Telos Testnet
		"https://telos.greymass.com/v1/chain/get_table_rows", // Telos Mainnet
		//"https://test.proton.eosusa.io/v1/chain/get_table_rows", // Proton Testnet (commented)
		"https://protontestnet.greymass.com/v1/chain/get_table_rows", // Proton Testnet
		"https://proton.cryptolions.io/v1/chain/get_table_rows", // Proton Mainnet
		//"https://test.wax.eosusa.io", // WAX Testnet (commented)
		"https://testnet.waxsweden.org/v1/chain/get_table_rows", // WAX Testnet
		"https://wax.greymass.com/v1/chain/get_table_rows" // WAX Mainnet
	};

	bool simpleAssets;

	const char* CURL_INIT_ERROR_MESSAGE = "CURL_INIT_ERROR";
	const char* CURL_REQUEST_ERROR_MESSAGE = "CURL_REQUEST_ERROR";

	const char* GetUrl(const char* ownerChar, const char* authorChar, const char* categoryChar, int network)
	{
		string baseUrl = networkEndpoints[network];

		if (simpleAssets)
			//baseUrl = baseUrl + "assets/search?author=" + authorChar + "&owner=" + ownerChar + "&category=" + categoryChar + "&page=1&limit=1000&sortField=assetId&sortOrder=asc";
			baseUrl = baseUrl + "/v1/chain/get_account";
		else
			baseUrl = baseUrl + "history/get_deltas?code=simpleassets&scope=" + ownerChar;

		return MakeStringCopy(baseUrl.c_str());
	}

	const char* GetCurlResponse(const char* url, char* owner)
	{
		CURL* curl;
		CURLcode response;
		//string readBuffer;
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
					strcat_s(author, strlen(name) + 1, name);
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

	const char* GetLicenseFromJsonData(const char* curlResponse, const char* licenseCheck, char* category)
	{
		JsonDataContainer jsonData = json::parse(curlResponse);

		// Si no tenemos datos
		if (jsonData.get_rows().size() == 0)
			return "ERROR-NO-NFTS";

		// Indica si presenta la licencia
		bool license = false;

		// Vamos filtrando entre las Row del json
		for (Row row : jsonData.get_rows())
		{
			// Comparamos si contiene la misma categoría
			// Y en caso afirmativo, comparamos con la licencia
			if (strcmp(row.get_category().c_str(), category) == 0)
			{
				string licenseData = row.get_author() + row.get_idata().get_name();
				if (strcmp(licenseData.c_str(), licenseCheck) == 0) {
					license = true;
					break;
				}
				else
					license = false;
			}
		}

		// Si tiene la licencia
		if (license)
			return "LICENSE";
		// Si no
		else
			return "ERROR LICENSE";

	}

	const char* CheckLicense(char* owner, char* author, char* category, char* license_name, int network)
	{
		const char* authorChar = MakeStringCopy(author);
		const char* ownerChar = MakeStringCopy(owner);
		const char* categoryChar = MakeStringCopy(category);
		const char* license_nameChar = MakeStringCopy(license_name);

		//const char* url = GetUrl(ownerChar, authorChar, categoryChar, network);
		const char* url = MakeStringCopy(networkEndpoints[network].c_str());

		const char* curlResponse = GetCurlResponse(url, owner);
		if (strcmp(curlResponse, CURL_INIT_ERROR_MESSAGE) == 0) return CURL_INIT_ERROR_MESSAGE;
		if (strcmp(curlResponse, CURL_REQUEST_ERROR_MESSAGE) == 0) return CURL_REQUEST_ERROR_MESSAGE;

		string authorC = author;
		string license_nameC = license_name;
		string licenseCheck = authorC + license_nameC;
		//char* licenseCheck = (char*)malloc(strlen(authorChar) + strlen(license_nameChar) + 1);
		////strcpy(licenseCheck, authorChar);
		//strcpy_s(licenseCheck, strlen(authorChar) + strlen(license_nameChar) + 1, authorChar);
		////strcpy(licenseCheck + strlen(authorChar), license_nameChar);
		//strcpy_s(licenseCheck + strlen(authorChar), strlen(authorChar) + strlen(license_nameChar) + strlen(authorChar) + 1, license_nameChar);

		return GetLicenseFromJsonData(curlResponse, licenseCheck.c_str(), category);
		//return GetLicenseFromJson(curlResponse, licenseCheck);
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
		checkNice1GenesisKey = 0;
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



	EXPORT_API const char* GetJsonData(int network, char* category, char* author, char* license_name)
	{
		string res = "";

		string authorChar = MakeStringCopy(author);
		string license_nameChar = MakeStringCopy(license_name);

		string licenseCheck = authorChar + license_nameChar;

		const char* networkEndpoint = networkEndpoints[network].c_str();
		//networkEndpoint = "http://jungle4.greymass.com/v1/chain/get_table_rows";
		CURL* curl;
		CURLcode response;
		unique_ptr<string> httpData(new string());
		string jsonstr = "{\r\n    \"json\": true,\r\n    \"code\": \"simpleassets\",\r\n    \"scope\": \"niceonetest1\",\r\n    \"table\": \"sassets\",\r\n    \"index position\": \"secondary\",\r\n    \"key type\": \"name\",\r\n    \"lower bound\": \"niceonedemos\",\r\n    \"upper bound\": \"niceonedemos\",\r\n    \"limit\": 100,\r\n    \"reverse\": false,\r\n    \"show_payer\": false\r\n}";
		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
			curl_easy_setopt(curl, CURLOPT_URL, networkEndpoint);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
			struct curl_slist* headers = NULL;
			headers = curl_slist_append(headers, "Content-Type: application/json");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr.c_str());
			response = curl_easy_perform(curl);

			if (response == CURLE_OK) {
				char* result = MakeStringCopy(httpData.get()->c_str());
				JsonDataContainer j = json::parse(result);

				int contCategory = 0;
				int contLicense = 0;
				for (Row row : j.get_rows())
				{
					char* incategory = MakeStringCopy(row.get_category().c_str());
					if (strcmp(incategory, category) == 0) {
						contCategory++;

						string authorRow = row.get_author();
						string idata = row.get_idata().get_name();

						string licenseData = authorRow + idata;

						res += "Licencia: " + to_string(contCategory) + ": " + licenseData + "\n";

						if (strcmp(licenseData.c_str(), licenseCheck.c_str()) == 0)
							contLicense++;

					}
				}

				res += "Hay " + to_string(contCategory) + " de la misma categoria.\n";
				string license = licenseCheck;
				res += "La licencia es: " + license + "\n";
				res += "Hay " + to_string(contLicense) + " de la misma licencia.\n";
			}
		}
		curl_easy_cleanup(curl);

		return MakeStringCopy(res.c_str());
	}
}