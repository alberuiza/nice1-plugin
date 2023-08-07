# Extended EOS networks support


<p style="color: red; font-weight: bold">>>>>>  gd2md-html alert:  ERRORs: 0; WARNINGs: 0; ALERTS: 1.</p>
<ul style="color: red; font-weight: bold"><li>See top comment block for details on ERRORs and WARNINGs. <li>In the converted Markdown or HTML, search for inline alerts that start with >>>>>  gd2md-html alert:  for specific instances that need correction.</ul>

<p style="color: red; font-weight: bold">Links to alert messages:</p><a href="#gdcalert1">alert1</a>

<p style="color: red; font-weight: bold">>>>>> PLEASE check and correct alert issues and delete this message and the inline alerts.<hr></p>


This extension is configured in both the C++ plugin and the _WalletManager.cs_ class of the Nice1 project.

In the C++ plugin, network endpoints are defined into an array of strings:


```
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
```


In _WalletManager.cs_ they are defined in the following _enum _class:


```
    public enum Network
    {
        Jungle4_Testnet, Eos_Mainnet, Proton_Testnet, Proton_Mainnet, Wax_Testnet, Wax_Mainnet, Telos_Testnet, Telos_Mainnet
    }
```


As can be seen, each enumerated value is placed in a way that corresponds to the string in the C++ plugin by index.

To select the network, you only need to choose it in the _Network _section of the script in the inspector.



<p id="gdcalert1" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image1.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert2">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](https://drive.google.com/uc?id=1JfisZhoK0u4SjRTTJt2I6GoPHfrA3m6q  "choose it in the _Network _section of the script in the inspector")


To test those networks, first, you need to configure Anchor by clicking on TestNet -> Manage Blockchains, and then enter the following private keys for each network:



* **Jungle4 - niceonetest1:** -
* **WAX - niceonedemos:** -
* **Proton - niceonedemos:** -
* **Telos - niceonedemos:** -

Note: Telos is not supported as it does not have the _simpleassets _contract.

Inside _Nice1Sample_ scene, we need to select the _WalletManager _GameObject and in its corresponding script from the inspector, we must enter the data within _License - Mandatory fields_ as shown in the previous image. If the connection is successful, after running the scene and clicking on the _Login_ button and logging in from Anchor, the author’s name will appear on the screen. Otherwise, it will display “No license”.
