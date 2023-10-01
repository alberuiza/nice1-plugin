# nice1plugin

# WELCOME TO NICE1

## Introduction

### About NICE1
NICE1 is a blockchain project, which aims to decentralize the video games industries. We offer a new NFT feature, that provides a valuable system of video games sales. 

The goal of NICE1 is to offer multi-services to all worldwide developers, related to the blockchain system of NFT, based on EOS platform. We are looking for a fair and revolutionary video game industry, where the player is the main character, owning every content that he buys or gets from the game, skipping main industries and bigger companies platform barriers. The player must be the realowner of his game content!

## NICE1 DevPortal

Documentation for developers: https://docs.nice1.dev/


# UnitTest - Nice1 plugin
    TestMakeStringCopy:
        Method Name: TestMakeStringCopy
        Purpose: This test is likely testing a function named MakeStringCopy. It checks whether this function correctly creates a copy of a C-style string and whether the copy matches the original.
        Test Steps:
            It defines a constant C-style string str with the value "Test."
            It calls the MakeStringCopy function on str and stores the result in output.
            It uses Assert::AreEqual to compare str and output to ensure they are equal.

    TestWriteCallback:
        Method Name: TestWriteCallback
        Purpose: This test is likely testing a function named WriteCallback. It checks whether this function correctly writes data to a string, mimicking a callback.
        Test Steps:
            It creates a std::string called str with the value "Test."
            It defines an empty std::string called output.
            It calls the WriteCallback function with parameters to write data from str to output.
            It uses Assert::AreEqual to compare str and output to ensure they are equal.

    TestGetCurlResponse:
        Method Name: TestGetCurlResponse
        Purpose: This test likely tests a function called GetCurlResponse, which makes HTTP requests and returns the response. It checks whether the function produces the expected responses for different URLs and network configurations.
        Test Steps:
            It makes several calls to GetCurlResponse with different URLs and stores the results in output.
            It uses Assert::AreEqual to compare the actual JSON responses with expected JSON responses for different URLs and network configurations.

    TestCheckLicense:
        Method Name: TestCheckLicense
        Purpose: This test likely tests a function called CheckLicense, which appears to validate licenses based on various parameters. It checks whether the function produces the expected results for different network configurations.
        Test Steps:
            It sets various input parameters like ownerChar, authorChar, categoryChar, and license_nameChar.
            It iterates over different network configurations (0, 2, 4) and calls CheckLicense with these parameters.
            It uses Assert::AreEqual to compare the actual output messages with expected messages for different configurations.

    TestCheckNice1GenesisKey:
        Method Name: TestCheckNice1GenesisKey
        Purpose: This test likely tests a function called CheckNice1GenesisKey, which appears to check for the presence of a Nice1 Genesis Key. It checks whether the function produces the expected results for different network configurations.
        Test Steps:
            It sets the ownerChar parameter.
            It iterates over different network configurations (0, 2, 4) and calls CheckNice1GenesisKey with the ownerChar.
            It uses Assert::AreEqual to compare the actual output messages with expected messages for different configurations.

    TestCheckLicensePlugin:
        Method Name: TestCheckLicensePlugin
        Purpose: This test likely tests a function called CheckLicensePlugin, which seems to be an extended version of CheckLicense with additional parameters. It checks whether the function produces the expected results for different configurations.
        Test Steps:
            It sets various input parameters like ownerChar, authorChar, categoryChar, license_nameChar, and checkNice1GenesisKey.
            It iterates over different network configurations (0, 2, 4) and calls CheckLicensePlugin with these parameters.
            It uses Assert::AreEqual to compare the actual output messages with expected messages for different configurations.


# UnitTest - Nice1 plugin Unity side
The `WalletManagerTestScript` class is responsible for defining and running unit tests for the `WalletManager` and related classes. It uses the NUnit testing framework to perform these tests. Let's dive into the details of this class:
Namespace and Class Declaration:
	The class is defined within the `Tests` namespace, which suggests that it contains tests for some functionality in your project.

Test Methods:
   - `InitializeTest` Method:
     - Inside the method, it creates an instance of the `WalletManager` class and calls its `Initialize` method.
     - After initialization, it asserts that the `hasNice1Key` property of the `WalletManager` instance is `false`, which means it checks whether initialization sets this property correctly.

   - `WalletAccountInitializeTest` Method:
     - This test method checks the `Initialize` method of the `WalletAccount` class.
     - It creates a `WalletAccount` instance and initializes it with some test data.
     - It then uses assertions to verify that the properties of the `WalletAccount` instance have been correctly set to the test data.

   - `LicenseOKTest` Method:
     - This test checks the `LicenseOK` method of the `WalletManager` class.
     - It creates an instance of `WalletManager` and asserts that the return value of `LicenseOK` is equal to the string "LICENSE".

   - `NO_LicenseTest` Method:
     - This test checks the `NO_License` method of the `WalletManager` class.
     - It creates an instance of `WalletManager` and asserts that the return value of `NO_License` is equal to the string "NO LICENSE".

   - `SetAccountTest` Method:
     - This test method is more complex and checks the `SetAccount` method of the `WalletManager` class under various conditions.
     - It sets up a `WalletManager` instance and configures different scenarios by changing properties such as `freeLicense_bool` and `IDATA_NAME`.
     - It iterates through network values and owner names, calling `SetAccount` and asserting the expected return values based on the configured scenarios.

