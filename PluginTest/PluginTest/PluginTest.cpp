#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include "JsonDataContainer.h"

using namespace quicktype;
using namespace std;
using json = nlohmann::json;

string networkEndpoint = "http://jungle4.greymass.com/v1/chain/get_table_rows";

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

// This function converts a boolean value to a character string
// representring "true" or "false"
inline const char* BoolToString(bool b)
{
	return b ? "true" : "false";
}

// Converts the container attribute of the Row class to character
// string
inline const string RowContainerToString(Row r) {
	string s = "";

	for (int i = 0; i < r.get_container().size(); i++) {
		s += r.get_container().at(i);
		if (i < r.get_container().size() - 1)
			s += ",";
	}

	return s;
}

// Converts the containterf attribute of the Row class to character
// string
inline const string RowContainerfToString(Row r) {
	string s = "";

	for (int i = 0; i < r.get_containerf().size(); i++) {
		s += r.get_containerf().at(i);
		if (i < r.get_containerf().size() - 1)
			s += ",";
	}

	return s;
}

// Generate a file with a given name and copy the data
// from a JsonDataContainer object into it, 
// creating an intermediate JSON for this purpose
void PutJsonDataIntoFile(string fileName, JsonDataContainer w) {
	// We create a file
	ofstream file(fileName);
	// We create a JSON
	json data;
	// We introduce the data from the JsonDataContainer into the JSON
	to_json(data, w);
	// And put the data into the file
	file << data;
}


// Generate a file with a given name by parsing to string
// the data of a JsonDataContainer
void PutDataIntoFile(string fileName, JsonDataContainer w) {
	// We create a file
	ofstream file(fileName);

	// Data has the following structure:
	// {"more":...,
	//"next_key":...,
	//"rows":[
	//{
	//"author":...,
	//"category":...,
	//"container":[...],
	//"containerf":[...],
	//"id":...,
	//"idata":...,
	//"mdata":...,
	//"owner":...,
	//},...
	//]
	//}
	string res = "";
	res += "{\"more\":";
	res += BoolToString(w.get_more());
	res += ",\"next_key\":";
	res += "\"" + w.get_next_key() + "\"";
	res += ",\"rows\":[";

	for (int i = 0; i < w.get_rows().size(); i++) {
		Row r = w.get_rows().at(i);

		res += "{\"author\":";
		res += "\"" + r.get_author() + "\"";

		res += ",\"category\":";

		res += "\"" +
			r.get_category()
			+ "\"";

		res += ",\"container\":[";
		res += RowContainerToString(r);
		res += "],\"containerf\":[";
		res += RowContainerfToString(r);
		res += "],\"id\":";
		res += "\"" + r.get_id() + "\",";
		res += "\"idata\":";

		res += "\"" +
			AddBarBeforeDoubleQuote(r.get_idata().get_complete())
			+ "\"";

		res += ",\"mdata\":";
		res += "\"" +
			AddBarBeforeDoubleQuote(r.get_mdata())
			+ "\",";
		res += "\"owner\":";
		res += "\"" + r.get_owner() + "\"";
		res += "}";

		if (i < w.get_rows().size() - 1)
			res += ",";
	}

	res = res + "]}";

	file << res;
}

// This function compares two text files to determine if their content
// is identical
void CompareFiles(string fileName1, string fileName2) {
	ifstream file1, file2;
	file1.open(fileName1);
	file2.open(fileName2);

	if (file1.fail()) {
		cout << "Error opening " + fileName1 << endl;
		cin.ignore(INT_MAX);
		exit(EXIT_FAILURE);
	}
	else if (file2.fail()) {
		cout << "Error opening " + fileName2 << endl;
		cin.ignore(INT_MAX);
		exit(EXIT_FAILURE);
	}

	string str1, str2;

	while (file1 && file2) {
		if (file1 && file2) {
			str1 += (char)file1.get();
			str2 += (char)file2.get();
		}
	}

	if (str1.compare(str2) == 0)
		cout << "Both files are identical" << endl;
	else
		cout << "There are differences between the files" << endl;

	file1.close();
	file2.close();
}


int main()
{
	// We initialize CURL
	CURL* curl;
	CURLcode res;
	unique_ptr<string> httpData(new string());

	string own = "niceonetest1";

	string jsonstr = "{\r\n    \"json\": true,\r\n    \"code\": \"simpleassets\",\r\n    \"scope\": \"" + own + "\",\r\n    \"table\": \"sassets\",\r\n    \"index position\": \"secondary\",\r\n    \"key type\": \"name\",\r\n    \"lower bound\": \"niceonedemos\",\r\n    \"upper bound\": \"niceonedemos\",\r\n    \"limit\": 100,\r\n    \"reverse\": false,\r\n    \"show_payer\": false\r\n}";

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	// If it initializes successfully
	if (curl) {
	// We connect to the API
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_URL, networkEndpoint.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr.c_str());
		res = curl_easy_perform(curl);

		// If the connection was successful
		if (res == CURLE_OK) {
			// We perform test
			char* result = MakeStringCopy(httpData.get()->c_str());

			string fileName1 = "file.txt",
				fileName2 = "file2.txt";

			JsonDataContainer j = json::parse(result);

			// We perform each test on each file 
			PutJsonDataIntoFile(fileName1, j);
			PutDataIntoFile(fileName2, j);

			// And finally, we compare if their contents match
			CompareFiles(fileName1, fileName2);

		}
	}
	curl_easy_cleanup(curl);

	return 0;
}
