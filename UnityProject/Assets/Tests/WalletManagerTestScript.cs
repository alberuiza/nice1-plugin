using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using NUnit.Framework;
using UnityEditor.VersionControl;
using UnityEngine;

namespace Tests
{
    public class WalletManagerTestScript
    {

        [Test]
        public void InitializeTest()
        {
            WalletManager walletManager = new WalletManager();
            walletManager.Initialize();

            Assert.IsFalse(walletManager.hasNice1Key);
        }

        [Test]
        public void WalletAccountInitializeTest()
        {
            string test = "nice1";
            WalletAccount walletAccount = new WalletAccount();

            walletAccount.Initialize(test, test, test, test, true, test);

            Assert.AreEqual(test, walletAccount.name);
            Assert.AreEqual(test, walletAccount.authority);
            Assert.AreEqual(test, walletAccount.publicKey);
            Assert.AreEqual(test, walletAccount.blockChain);
            Assert.IsTrue(walletAccount.isHardware);
            Assert.AreEqual(test, walletAccount.chainID);
        }

        [Test]
        public void LicenseOKTest()
        {
            WalletManager walletManager = new WalletManager();
            Assert.AreEqual("LICENSE", walletManager.LicenseOK());
        }

        [Test]
        public void NO_LicenseTest()
        {
            WalletManager walletManager = new WalletManager();
            Assert.AreEqual("NO LICENSE", walletManager.NO_License());
        }

        [Test]
        public void SetAccountTest()
        {
            // Preparing data
            WalletManager walletManager = new WalletManager();
            walletManager.Initialize();
            var values = Enum.GetValues(typeof(WalletManager.Network));
            string owner = "";

            // FREE LICENSES
            walletManager.freeLicense_bool = true;

            // Message: "LICENSE"
            for (int i = 0; i < values.Length - 2; i += 2)
            {
                walletManager.network = (WalletManager.Network)values.GetValue(i);
                Assert.AreEqual("LICENSE", walletManager.SetAccount(owner));
            }

            // NON-FREE LICENSES
            walletManager.freeLicense_bool = false;

            // WITHOUT NICE1 GENESIS KEY
            // Message: "NO LICENSE"
            walletManager.IDATA_NAME = "No license";

            for (int i = 0; i < values.Length - 2; i += 2)
            {
                if (i == 0)
                    owner = "niceonetest1";
                else
                    owner = "niceonedemos";

                Assert.AreEqual("NO LICENSE", walletManager.SetAccount(owner));
            }

            // Message: "LICENSE"
            walletManager.IDATA_NAME = "GameKey";

            for (int i = 0; i < values.Length - 2; i += 2)
            {
                walletManager.network = (WalletManager.Network)values.GetValue(i);

                if (i == 0)
                    owner = "niceonetest1";
                else
                    owner = "niceonedemos";

                Assert.AreEqual("LICENSE", walletManager.SetAccount(owner));
            }

            // WITH NICE1 GENESIS KEY
            walletManager.checkNice1GenesisKey = 1;

            // Message: "NO LICENSE"
            walletManager.IDATA_NAME = "No license";
            owner = "testnice1";

            for (int i = 0; i < values.Length - 2; i += 2)
            {
                Assert.AreEqual("NO LICENSE", walletManager.SetAccount(owner));
            }

            // Message: "LICENSE"
            walletManager.IDATA_NAME = "testnice1";

            for (int i = 0; i < values.Length - 2; i += 2)
            {
                walletManager.network = (WalletManager.Network)values.GetValue(i);

                if (i == 0)
                    owner = "niceonetest1";
                else
                    owner = "niceonedemos";

                Assert.AreEqual("LICENSE", walletManager.SetAccount(owner));
            }

        }

    }

    public class WalletManager
    {
        #region DLL Imports

        [DllImport("Nice1Plugin", CallingConvention = CallingConvention.Cdecl)]
        private static extern int CheckLicense(string owner, string author, string category, string license_name, string idata_name);

        [DllImport("Nice1Plugin", CallingConvention = CallingConvention.Cdecl)]
        private static extern int CheckNice1GenesisKey(string owner, string author, string category, string license_name, string idata_name, int checkNice1GenesisKey);

        [DllImport("Nice1Plugin", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern IntPtr CheckLicensePlugin(string owner, string author, string category, string license_name, int checkNice1GenesisKey, int network);

        #endregion

        #region License Parameters

        public enum Network
        {
            Jungle4_Testnet, Eos_Mainnet, Proton_Testnet, Proton_Mainnet, Wax_Testnet, Wax_Mainnet, Telos_Testnet, Telos_Mainnet
        }

        [Header("Network")]
        public Network network;
        // Proton testnet: https://proton-testnet.eosio.online/endpoints
        private List<string> networkEndpoints = new List<string>
    {
        "https://jungle4.greymass.com/v1/",
        "https://eos.greymass.com/v1/",
        "https://protontestnet.greymass.com/v1/",
        "https://proton.cryptolions.io/v1/",
        "https://testnet.waxsweden.org/v1/",
        "https://wax.greymass.com/v1/",
        "https://test.telos.eosusa.io/v1/",
        "https://telos.greymass.com/v1/"
    };

        [Header("License - Mandatory fields")]
        public string AUTHOR = "niceonedemos";
        public string IDATA_NAME = "GameKey";
        public string CATEGORY = "llegends";

        [Header("Nice1 Genesis Key")]
        public bool checkNice1GenesisKey_bool;

        [Header("FreeToPlay")]
        public bool freeLicense_bool;

        [Header("License - Error message")]
        public string errorLicenseText = "You are not licensed to use this game";

        [Header("Vuplex - Error message")]
        public string errorVuplexText = "It is necessary to have the Vuplex plugin installed. For more information go to https://docs.nice1.dev/";

        [HideInInspector]
        public int checkNice1GenesisKey;

        [HideInInspector]
        public bool hasNice1Key { get; private set; }

        [HideInInspector]
        public string owner { get; private set; }

        #endregion

        public WalletAccount CurrentAccount { get; private set; }

        private bool simpleAssets;
        private WebRequestResults lastRequest;
        private WebRequestResultsDeltas lastRequestDeltas;

        public delegate void WalletDelegate();
        public static event WalletDelegate OnNoLicense;

        // We substitute Awake() method for this method
        public void Initialize()
        {
            hasNice1Key = false;
            owner = "";

            CurrentAccount = new WalletAccount();
            checkNice1GenesisKey = checkNice1GenesisKey_bool ? 1 : 0;
        }

        public string SetAccount(string name)
        {
            string res = "";

            CurrentAccount.Initialize(name, null, null, null, false, null);
            if (freeLicense_bool)
            {
                res = LicenseOK();
            }
            else
                res = SearchAssetsByOwner(name);

            return res;
        }

        private string SearchAssetsByOwner(string owner)
        {
            string licenseResult = Marshal.PtrToStringAnsi(CheckLicensePlugin(owner, AUTHOR, CATEGORY, IDATA_NAME, checkNice1GenesisKey, (int)network));

            string res = "";

            if (licenseResult == "LICENSE" || licenseResult == "NICE1KEY")
            {
                if (licenseResult.Equals("NICE1KEY"))
                    hasNice1Key = true;
                this.owner = owner;

                res = LicenseOK();
            }
            else
            {
                res = NO_License();
            }

            return res;
        }

        public string LicenseOK()
        {
            return "LICENSE";
        }

        public string NO_License()
        {

            return "NO LICENSE";
        }

    }

    #region Data Model 

    [System.Serializable]
    public class WalletAccount
    {
        public void Initialize(string name, string authority, string publicKey, string blockChain, bool isHardware, string chainID)
        {
            this.name = name;
            this.authority = authority;
            this.publicKey = publicKey;
            this.blockChain = blockChain;
            this.isHardware = isHardware;
            this.chainID = chainID;
        }

        public string name;
        public string authority;
        public string publicKey;
        public string blockChain;
        public bool isHardware;
        public string chainID;
    }

    #region SimpleAssets Data Model

    [System.Serializable]
    public class WebRequestResults
    {
        public List<WebResultContainer> results;
    }

    [System.Serializable]
    public class WebResultContainer
    {
        public string assetId;
        public string author;
        public string owner;
        public string category;
        public string control;
        public ImmutableData idata;
        public MutableData mdata;
    }

    [System.Serializable]
    public class ImmutableData
    {
        public string name;
        public string img;
    }

    [System.Serializable]
    public class MutableData
    {
        public string name;
    }

    #endregion

    #region Deltas Data Model

    [System.Serializable]
    public class WebRequestResultsDeltas
    {
        public List<WebResultContainerDeltas> deltas;
    }

    [System.Serializable]
    public class WebResultContainerDeltas
    {
        public DataContainerDeltas data;
    }

    [System.Serializable]
    public class DataContainerDeltas
    {
        public string author;
        public string category;
        public string owner;
        public string idata;
    }

    #endregion

    #endregion

}
