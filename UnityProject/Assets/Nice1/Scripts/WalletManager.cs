using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.UI;
using System.Runtime.InteropServices;
using System;

public class WalletManager : Singleton<WalletManager>
{
    #region DLL Imports

    [DllImport("Nice1Plugin", CallingConvention = CallingConvention.Cdecl)]
    private static extern int CheckLicense(string owner, string author, string category, string license_name, string idata_name);

    [DllImport("Nice1Plugin", CallingConvention = CallingConvention.Cdecl)]
    private static extern int CheckNice1GenesisKey(string owner, string author, string category, string license_name, string idata_name, int checkNice1GenesisKey);

    [DllImport("Nice1Plugin", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern IntPtr CheckLicense2TEST(string owner, string author, string category, string license_name, int checkNice1GenesisKey, int network);

    [DllImport("Nice1Plugin", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern IntPtr GetJsonData();

    #endregion

    #region UI Objects

    [Header("Login")]
    public UIPanel loginPanel;
    public GameObject loginPanelGO;
    public GameObject loginButton;
    public GameObject logoutButton;
    public UIPanel userPanel;
    public Text textUserAccount;

    #endregion

    #region License Parameters

    public enum Network
    {
        Jungle4_Testnet, Eos_Mainnet, Telos_Testnet, Telos_Mainnet, Proton_Testnet, Proton_Mainnet, Wax_Testnet, Wax_Mainnet
    }

    [Header("Network")]
    public Network network;
    // Proton testnet: https://proton-testnet.eosio.online/endpoints
    // Telos testnet: https://telos-testnet.eosio.online/endpoints
    private List<string> networkEndpoints = new List<string>
    {
        "https://jungle4.greymass.com/",
        "https://eos.api.simpleassets.io/v1/",
        "https://telostest.eu.eosamsterdam.net/v1",
        "https://telos.api.simpleassets.io/v1/",
        "https://test.proton.eosusa.news/v2/",
        "https://proton.greymass.com/v1/",
        "https://testnet.waxsweden.org/v2/",
        "https://wax.eu.eosamsterdam.net/v2/"
    };

    [Header("License - Mandatory fields")]
    public string AUTHOR = "niceonedemos";
    public string IDATA_NAME = "GAME LICENSE - LegendaryLegends";
    public string CATEGORY = "llegends";

    [Header("Nice1 Genesis Key")]
    public bool checkNice1GenesisKey_bool;

    [Header("License - Error message")]
    public string errorLicenseText = "You are not licensed to use this game";

    [Header("Vuplex - Error message")]
    public string errorVuplexText = "It is necessary to have the Vuplex plugin installed. For more information go to https://docs.nice1.dev/";

    [HideInInspector]
    public int checkNice1GenesisKey;

    #endregion

    public WalletAccount CurrentAccount { get; private set; }

    private bool simpleAssets;
    private WebRequestResults lastRequest;
    private WebRequestResultsDeltas lastRequestDeltas;

    public delegate void WalletDelegate();
    public static event WalletDelegate OnNoLicense;

    private void Awake()
    {
        CurrentAccount = new WalletAccount();
        checkNice1GenesisKey = checkNice1GenesisKey_bool ? 1 : 0;
    }

    #region License 

    public void SetAccount(string name)
    {
        CurrentAccount.Initialize(name, null, null, null, false, null);
        StartCoroutine(SearchAssetsByOwner(name));
    }

    public void PruebaJson()
    {
        StartCoroutine(CorrutinaPruebaJson());
    }

    private IEnumerator CorrutinaPruebaJson()
    {
        string idataOne = Marshal.PtrToStringAnsi(GetJsonData());

        Debug.Log(idataOne);

        yield return new WaitForSeconds(0f);
    }

    private IEnumerator SearchAssetsByOwner(string owner)
    {
        string licenseResult = Marshal.PtrToStringAnsi(CheckLicense2TEST(owner, AUTHOR, CATEGORY, IDATA_NAME, checkNice1GenesisKey, (int)network));

        Debug.Log(licenseResult);
        if (licenseResult == "LICENSE" || licenseResult == "NICE1KEY")
        {
            LicenseOK();
        }
        else
        {
            NO_License();
        }

        yield return new WaitForSeconds(0);
    }

    #region Deprecated

    //private IEnumerator SearchAssetsByOwner(string owner)
    //{
    //string url;
    //if (checkNice1GenesisKey == 1)
    //{
    //    Debug.Log("CHECK NICE1 GENESIS KEY");
    //    url = GetURL("niceonechain", owner, "niceoneepics");
    //    yield return StartCoroutine(GetRequest(url));
    //    GetLicense(owner, checkNice1GenesisKey: 1);
    //    if (license == false)
    //    {
    //        Debug.Log("CHECK GAME LICENSE");
    //        url = GetURL(AUTHOR, owner, CATEGORY);
    //        yield return StartCoroutine(GetRequest(url));
    //        GetLicense(owner, checkNice1GenesisKey: 0);
    //    }
    //}
    //else
    //{
    //    Debug.Log("CHECK GAME LICENSE");
    //    url = GetURL(AUTHOR, owner, CATEGORY);
    //    yield return StartCoroutine(GetRequest(url));
    //    GetLicense(owner, checkNice1GenesisKey: 0);
    //}
    //}

    //private string GetURL(string author, string owner, string category)
    //{
    //    string networkEndpoint = networkEndpoints[(int)network];

    //    switch (network)
    //    {
    //        case Network.Jungle3_Testnet:
    //        case Network.Eos_Mainnet:
    //        case Network.Telos_Mainnet:
    //            simpleAssets = true;
    //            return networkEndpoint + "assets/search?author=" + author + "&owner=" + owner + "&category=" + category + "&page=1&limit=1000&sortField=assetId&sortOrder=asc";

    //        case Network.Telos_Testnet:
    //        case Network.Proton_Mainnet:
    //        case Network.Proton_Testnet:
    //        case Network.Wax_Testnet:
    //        case Network.Wax_Mainnet:
    //            simpleAssets = false;
    //            return networkEndpoint + "history/get_deltas?code=simpleassets&scope=" + owner;
    //    }

    //    return "";
    //}

    //bool license = false;

    //void GetLicense(string owner, int checkNice1GenesisKey)
    //{
    //    if (simpleAssets && lastRequest != null && lastRequest.results.Count > 0)
    //    {
    //        for (int i = 0; i < lastRequest.results.Count; i++)
    //        {
    //            Debug.Log(lastRequest.results[i].idata.name);
    //            CheckLicense(owner, lastRequest.results[i].author, lastRequest.results[i].idata.name, checkNice1GenesisKey);
    //            if (license) break;
    //        }
    //    }
    //    else if (!simpleAssets && lastRequestDeltas != null && lastRequestDeltas.deltas.Count > 0)
    //    {
    //        for (int i = 0; i < lastRequestDeltas.deltas.Count; i++)
    //        {
    //            string idata = lastRequestDeltas.deltas[i].data.idata;
    //            string idata_name = GetNameFromIData(idata);

    //            Debug.Log(idata_name);
    //            CheckLicense(owner, lastRequestDeltas.deltas[i].data.author, idata_name, checkNice1GenesisKey);
    //            if (license) break;
    //        }
    //    }

    //    if (simpleAssets) Debug.Log("results count: " + lastRequest.results.Count);
    //    else Debug.Log("results count: " + lastRequestDeltas.deltas.Count);

    //    if (license)
    //    {
    //        LicenseOK();
    //    }
    //    else
    //    {
    //        NO_License();
    //    }
    //}

    //private void CheckLicense(string owner, string author, string idata_name, int checkNice1GenesisKey)
    //{
    //    int textStr;
    //    if (checkNice1GenesisKey == 1)
    //        textStr = CheckNice1GenesisKey(owner, author, CATEGORY, IDATA_NAME, idata_name, checkNice1GenesisKey);
    //    else
    //        textStr = CheckLicense(owner, AUTHOR, CATEGORY, IDATA_NAME, idata_name);
    //    if (textStr == 1)
    //    {
    //        license = true;
    //    }
    //}

    //private string GetNameFromIData(string idata)
    //{
    //    string name = idata;
    //    int index = name.IndexOf(':') + 1;
    //    name = name.Substring(index, name.Length - index - 1);

    //    string temp = "";
    //    foreach (char letter in name)
    //    {
    //        if (letter != '\\' && letter != '\"' && letter != '}') temp += letter;
    //    }

    //    return temp;
    //}

    //IEnumerator GetRequest(string uri)
    //{
    //    using (UnityWebRequest webRequest = UnityWebRequest.Get(uri))
    //    {
    //        // Request and wait for the desired page.
    //        yield return webRequest.SendWebRequest();

    //        string[] pages = uri.Split('/');
    //        int page = pages.Length - 1;

    //        switch (webRequest.result)
    //        {
    //            case UnityWebRequest.Result.ConnectionError:
    //            case UnityWebRequest.Result.DataProcessingError:
    //                Debug.LogError(pages[page] + ": Error: " + webRequest.error);
    //                break;
    //            case UnityWebRequest.Result.ProtocolError:
    //                Debug.LogError(pages[page] + ": HTTP Error: " + webRequest.error);
    //                break;
    //            case UnityWebRequest.Result.Success:
    //                Debug.Log(pages[page] + ":\nReceived: " + webRequest.downloadHandler.text);
    //                if (simpleAssets) lastRequest = JsonUtility.FromJson<WebRequestResults>(webRequest.downloadHandler.text);
    //                else lastRequestDeltas = JsonUtility.FromJson<WebRequestResultsDeltas>(webRequest.downloadHandler.text);

    //                break;

    //            default:
    //                break;
    //        }
    //    }
    //}

    #endregion

    public void LicenseOK()
    {
        // TO DO: write your code here when License is OK
        // EXAMPLE
        Debug.Log("LICENSE");
        HideLogin();
        SetLoggedInMenu();
        SetUserAccount(CurrentAccount.name);
        ShowUser();
        // Start your game
        // - Got to other scene
        // - Deactivate Canvas
    }

    public void NO_License()
    {
        // TO DO: write your code here when License is not OK
        Debug.Log("NO LICENSE");
        if (OnNoLicense != null) OnNoLicense();
        HideLogin();
        SetUserAccount("No License");
        ShowUser();
    }

    #endregion

    #region UI

    public void SetLoggedInMenu()
    {
        HideLogin();
        if (loginButton != null) loginButton.SetActive(false);
    }

    private void SetNotLoggedInMenu()
    {
        if (loginButton != null) loginButton.SetActive(true);
        if (logoutButton != null) logoutButton.SetActive(false);
    }

    public async void LogOut()
    {
#if VUPLEX_STANDALONE
        await GameObject.FindGameObjectWithTag("canvasWebView").GetComponent<Vuplex.WebView.CanvasWebViewPrefab>().LogOut2();
#endif
        SetNotLoggedInMenu();
    }

    public void ShowLogin()
    {
#if VUPLEX_STANDALONE
        if (loginPanel != null) ShowPanel(loginPanel);
#else
        Debug.Log(errorVuplexText);
#endif
    }

    public void HideLogin()
    {
        if (loginPanel != null) HidePanel(loginPanel);
    }

    public void ShowUser()
    {
        if (userPanel != null) ShowPanel(userPanel);
    }

    public void HideUser()
    {
        if (userPanel != null) HidePanel(userPanel);
    }

    private void ShowPanel(UIPanel panel)
    {
        if (panel != null) panel.ShowPanel();
    }

    private void HidePanel(UIPanel panel)
    {
        if (panel != null) panel.HidePanel();
    }

    public void SetUserAccount(string userAccount)
    {
        textUserAccount.text = userAccount;
    }

    #endregion

    #region Errors
    public void ShowLoginError(string errorMessage)
    {
        string errorText = "There was a problem logging into your account. Make sure you are logged into Scatter."
                         + "\nError: " + errorMessage;


        Debug.LogError("There was a problem logging into your account. Make sure you are logged into Scatter.");
        Debug.LogError("Error: " + errorMessage);
    }

    public void ShowApiError(string errorMessage)
    {
        string errorText = "There was a problem communicating with the API. Please try again."
                         + "\nError: " + errorMessage;


        Debug.LogError("There was a problem communicating with the API. Please try again.");
        Debug.LogError("Error: " + errorMessage);
    }
    #endregion
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