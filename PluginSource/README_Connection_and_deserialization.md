# Connection and deserialization

We use the code from _Plugin.cpp_ for connection and deserialization.

Based on the _[nlohmann/json](https://github.com/nlohmann/json)_ library, an auxiliary class _JsonDataContainer.h_ is created, which independently organizes the data obtained from the JSON and provides corresponding functions to retrieve them separately.

Here is a breakdown of the key components of its code:

**Header inclusion:**



* The code includes the necessary header files to use the _nlohmann/json _library, as well as other headers required for the code’s functionality.

**Namespace _quicktype_**:



* The _quicktype_ namespace is defined to group classes and functions related to JSON data processing.

**Utility functions:**



* Several utility functions are defined to facilitate JSON data handling:
    * _get_untyped_: Allows retrieving an untyped JSON object (of type _json_) from a JSON object with a specific key.
    * _AddBarBeforeDoubleQuote_: This function replaces all double quotes (_“_) within a string with _\”_.

**Class _Idata_:**



* Represents a JSON object containing identification data.
* It has methods to access and modify attributes of the JSON object, such as _complete_ (all data) and _name_ (a specific name within _idata_).

**Class _Row_:**



* Represents a JSON data row containing information about a record.
* Contains various attributes, such as _author_, _category_, _container_, _containerf_, _id_, _idata_, _mdata_ and _owner_.
* Provides methods to access and modify the mentioned attributes.

**Class _JsonDataContainer_:**



* Represents a JSON container containing multiple data rows (_Row_).
* Has attributes like _more_, _next_key_ and _rows_.
* The methods allow accessing and modifying these attributes.

**_from_json_ and _to_json_ functions:**



* These functions are used to convert JSON objects to objects of the defined classes (_Row _and _JsonDataContainer_) and vice versa.
* These functions are used in the process of deserialization (parsing) and serialization of JSON objects.

In summary, the provided code defines classes and functions to work with JSON data using the _nlohmann/json_ library. The _Row_ and _JsonDataContainer_ classes represent specific data structures for manipulating information contained in JSON objects. The utility functions ease the extraction of specific data from JSON objects.

Continuing with the code from _Plugin.cpp_, the connection to the API is performed from the function _GetCurlResponse_, which is responsible for making an HTTP request using the _curl_ library to fetch a response from a specific URL and returning the response data as a string (_const char*_). Here is a breakdown of the function’s flow: 

**Declare and initialize variables:**



* _curl_: A pointer to the CURL session handler used for making the HTTP request.
* _response_: Stores the response code of the HTTP request.
* _httpData_: A pointer to a _string _object allocated on the heap to store the received data in the HTTP response.

**Build the HTTP request body:**



* A character string called _jsonstr_ is created, containing a JSON object with several fields used for making the specific request. The values of these fields are initialized with specific data that corresponds to the plugin’s logic. In this case, the JSON object contains parameters to fetch information about NFT assets that meet certain conditions.

**Initialize and configure CURL:**



* The CURL global initialization is performed using _curl_global_init(CURL_GLOBAL_ALL)_.
* Then, _curl _is initialized using _curl_easy_init()_.
* Possible initialization errors are checked.

**Configure CURL request options:**



* The _curl_easy_setopt_ function is used to set various options for the CURL request.
* The request type is set as _POST_, the target URL, the option to follow redirects (_CURLOPT_FOLLOWLOCATION_), the default protocol (_https_), the write function for data (_WriteCallback_), and the pointer to data (_httpData_) where the HTTP response will be stored.
* The request header (_Content-Type_) is set, and the _jsonstr_ string is added as the request body (_CURLOPT_POSTFIELDS_).

**Execute the CURL request:**



* The HTTP request is performed using _curl_easy_perform_, and the result is stored in the _response_ variable.

**Cleanup CURL and handle errors:**



* CURL resources are cleaned up using _curl_easy_cleanup(curl)_ and _curl_global_cleanup()_.
* If the response is successful (_response == 0_), a copy of the received data is returned using _MakeStringCopy(httpData.get()->c_str())_.
* If the request fails, an error message with the response code (_response_) is returned using _MakeStringCopy(CURL_REQUEST_ERROR_MESSAGE + response)_.
* If CURL initialization fails (_curl_ is _NULL_), an error message is returned using _MakeStringCopy(CURL_INIT_ERROR_MESSAGE)_.

In summary, the _GetCurlResponse_ function is used to make an HTTP request to a specific URL using the _curl_ library. It then handles the obtained response and returns the response data or error messages based on the result of the request. It is crucial for fetching information from the blockchain and verifying licenses of NFTs on various blockchain networks.

The _GetLicenseFromJsonData _function is responsible for extracting license information from the JSON data obtained from the CURL response. Here is a breakdown of the function’s flow:

**Parse the JSON data:**



* The input parameter _curlResponse_ is a string containing the JSON data received from the CURL request.
* The function uses the _json::parse_ function from the _nlohmann/json.hpp_ library to parse the JSON data into a _JsonDataContainer_ object named _jsonData_. This object is used to access the rows and their properties in the JSON.

**Check for data presence:**



* The function first checks if the JSON data contains any rows (_jsonData.get_rows().size()_). If there are no rows, it means there is no relevant data, and the function returns “ERROR-NO-NFTS” to indicate that there are no NFTs for the given criteria.

**Iterating through JSON rows:**



* The function iterates through each row in the JSON data using a _for_ loop. The loop variable _row_ represents an individual row in the JSON data.

**Check for matching category:**



* For each row, the function compares the _category_ provided as input with the category (_row.get_category()_) stored in the row.
* If there is a match between the provided category and the row’s category, it means the row contains information about the relevant NFTs in the specified category.

**Compare with the license:**



* If there is a match in the category, the function proceeds to construct a unique string _licenseData_ by concatenating the author and the name of the NFT (_row.get_author() + row.get_idata().get_name()_).
* The _licenseCheck_ parameter is a string that represents the specific license to check.
* The function then compares _licenseData_ with _licenseCheck_ using _strcmp_. If they match, it means the specific license is present for the given NFT.

**Determine the result:**



* If the license is found (_license_ is true), the function returns “LICENSE” to indicate that the owner possesses the specific license for the NFT in the provided category.
* If the license is not found (_license _is false), the function returns “ERROR LICENSE” to indicate that the owner does not possess the specific license.

In summary, the _GetLicenseFromJsonData_ function takes the JSON data from the CURL response and checks if a specific license (_licenseCheck_) is present for NFTs in a given category. It iterates through the JSON rows, matches the category, and then compares with the specific license. If the license is found, it returns “LICENSE”; otherwise, it returns “ERROR LICENSE”. This function plays a crucial role in verifying licenses for NFTs obtained from the blockchain data.

Finally, for testing an auxiliary project called _PluginTest_ has been used where some functions differ from the plugin project.

Its code is a program that makes a connection to an API using the libcurl library and processes JSON data using the _nlohmann/json_ library.

Here is a breakdown of the main parts of the code:

**Header inclusion:** The program includes several necessary headers such as _&lt;iostream>_, _&lt;stdio.h>_, _&lt;string>_, _&lt;cstring>_, _&lt;curl/curl.h>_, _&lt;nlohmann/json.hpp>_ and _&lt;fstream>_.

**Namespace and Alias:** Namespaces are used to shorten the names of used libraries, for example, _quicktype_ for _nlohmann::json_ and _std_ for the C++ standart library. An alias _json _is created for _nlohmann::json_.

**Functions and Methods Definitions:**



* **_WriteCallback_: **This function is used as a callback for the libcurl library and is responsible for receiving data received by the HTTP request.
* **_MakeStringCopy_:** This function takes a character string and creates a dynamic copy of it in memory, returning a pointer to the new block of memory.
* **_BoolToString_: **This function converts a boolean value to a character string representing “true” or “false”.
* **_RowContainerToString_ and _RowContainerfToString_: **These functions convert the _container _and _containerf_ attributes of the _Row_ class to character strings, respectively.
* **_PutJsonDataIntoFile_ and _PutDataIntoFile_: **These functions write JSON data into text files with the specified names.
* **_CompareFiles_: **This function compares two text files to determine if their content is identical.

**_main _Function: **This is the main function of the program.



* We use the _libcurl_ library similarly as we do in the plugin.
* If the request is successful (without errors), the received data is parsed and stored in a _JsonDataContainer_ object.
* The received data is written to two text files using the _PutDataIntoFile_ and _PutDataIntoFile_ functions.
* Then, the two text files are compared using the _CompareFiles _function.

In summary, this program verifies the connection to the API using _libcurl _library, and checks that the serialization and deserialization of data are correct by directly inputting the JSON data into a file for comparison with another file where operations of the _JsonDataContainer _class are used to obtain that data.

To run the test project, open the _PluginTest.sln _file with Visual Studio, located in the path _nice1-plugin\PluginTest_.

Inside the main function, we enter the name of the owner we want to test into the “own” variable” (in our case, it will be “niceonetest1”):

int main()

{

	// We initialize CURL

	CURL* curl;

	CURLcode res;

	unique_ptr&lt;string> httpData(new string());

	string own = “niceonetest1”

Inside the project, after right-clicking, click on “Clean” and then on “Build”. After this, run the project, and the following message will appear in the console if both files are identical: “Both files are identical”

If there is any error in the data conversion, and therefore, the files are not identical, the following message will appear: “There are differences between the files”

Furthermore, inside the path _nice1-plugin\PluginTest\PluginTest_ the files _file1.txt_ and _file2.txt_ have been generated. which contain the information of the JSON and the JsonDataContainer, respectively.
