# Use of audited mechanics contracts

**SCENE:**

The scene _Nice1CharacterSelectorSample _is located in the _Assets|Nice1\Demos_ folder and contains the following elements:

![alt_text](https://drive.google.com/uc?id=1DfABCRlAyLuxXsZT6VO1wEsbN03L_REN "scene_Nice1CharacterSelectorSample")


The differences in respect with the example login scene _Nice1Sample_ (_Assets\Nice1\Demos\Nice1Sample_) are that the main panel contains the _CharacterSelectionScreen_ (_Assets\Nice1\Prefabs\CharacterSelectionScreen.prefab_) prefab instead of _UserPanel_, and that the _CharacterSelector _prefab_ _is located outside _Canvas_ with _WalletManager _as child.

**SCENE SETTINGS:**

To configurate the scene, we need to click on _CharacterSelector_, and enter our data inside the _ElementConfigurator _(_Assets/Nice1/Scripts/Selector/ElementsConfigurator.cs_) component. For example:

![alt_text](https://drive.google.com/uc?id=1f_XeJAlaeUkex8m0we0tl0CzlMtYT__p "enter_our_data_inside_the_ElementConfigurator")


Each element of _Element List_ is a _ScriptableData _(_Assets\Nice1\Scripts\Selector\Scriptables\ScriptableData.cs_) with the following information:



*  **_Label_: **It corresponds with the element’s name.
* **_Idata_: **It corresponds with the text associated to the license to check.
* **_Sprite_: **It corresponds with the sprite which will be used to represent visually the element in the UI.
* **_Free Element_: **If the box is checked, the element will be directly unlocked, without verifying any license.
*  **_Nice1 Key_: **It the box is checked, in case of the _Nice1GenesisKey_ option from _WalletManager_ is selected and verifying that the owner has the _GenesisKey_ after login, the license of the element will not be checked (this only runs when the scene is executed).

By pressing the button which contains the text “Save data into scriptable data list”, we can store the data in the _Scriptable Data List_ element, which is a _ScriptableDataList _(_Assets\Nice1\Scripts\Selector\Scriptables\ScriptableDataList.cs_), to use later in other scripts, or in case we need to test with other data keeping a backup copy. In our example, we take the generated ScriptableObject _Elements_ (_Assets\Nice1\Scripts\Selector\Scriptables\Stored data\Elements_).

Moreover, we can load the saved data from this element, by pressing the button which contains the text “Load data from scriptable data list”.


![alt_text](https://drive.google.com/uc?id=1OzaEbC05ICuPbHCvVlb1E5GsvnvwM4gv "load_data_from_scriptable_data_list")


Within _Settings _options, we see the followings elements:



* **_Display Mode_:** We can select between two options:
    * **_Transparent_**, if we intend to increase the element’s transparency in case of not having its license.
    * **_Show_**, if we intend to hide the element from the panel in case of not having its license.
* **_Test Owner_:** It contains the owner’s name to test their licenses without login (this option only runs in editor).
* **_Target_: **The _Canvas_’ element_ _which will contain the elements to be displayed. In our case, we select _CharacterContainer_, which will be explained later.
* **_Prefab_:_ _**The GameObject from which we generate the elements with the data we have introduced.

In our case, we use the _Character _(_Assets\Nice1\Prefabs\Character.prefab_) prefab, which is just a button which contains a text (_Text Mesh Pro_) as child:


![alt_text](https://drive.google.com/uc?id=1QjB3iAyf_hw9zh9fo-bpngFqPkdNJoKI "the_character_prefab")

After generating from our _Element Configurator_, the _CharacterLoader_ (_Assets\Nice1\Character\CharacterLoader.cs_) is added, in order to load the stored data that corresponds from _Element List_.

After entering our data, by pressing the button which contains the text “Create elements in the UI”, the elements will appear disorganized:

![alt_text](https://drive.google.com/uc?id=1ogbqZUL8BvMTcVY2ilG8u54UbleunH-e "create_elements_in_the_UI")


To bring order to the scene, we need to access the _CharacterContainer_ element, which is located as a child of _CharacterSelectionScreen_, inside _Panel_ of _Canvas_ and move each element as we want:


![alt_text](https://drive.google.com/uc?id=1SNw-wErq1b92unK04PbxaTy5BTnLtWoO "characterContainer_element")


![alt_text](https://drive.google.com/uc?id=1jKSXD3X9wmBYAaCshklinPbtD2RHkvWW "select_character")


After that, by clicking the button which contains the text “Refresh licenses”, we can check if the test owner has the licenses of each element and see how they are displaying according to the _Display Mode _selected. Here, our example:

**Transparent:**


![alt_text](https://drive.google.com/uc?id=12zUG1kdSuL_E-zalsZ9SxNvpGVjp0TQs "transparent")


**Show:**


![alt_text](https://drive.google.com/uc?id=1Y81LeelZ0pVW-8VpdnBdb_NVueFUITMU "show")


If we execute the scene, the character selector screen is not visible, and we just can see the login button.


![alt_text](https://drive.google.com/uc?id=1MGzpZmxxT6VlJrBHiWdFhrsEzWi7BQNJ "login_button")


After login, the panel appears and tries to check our licenses:


![alt_text](https://drive.google.com/uc?id=1IARwPX4tN_Ln6B9zF2fASAbEKorLSoGR "character_selection")


In case our data is not updated, we can press the green button of the right bottom corner to refresh licenses. For example, if we indicate that our character “Capsule” is free and we click the green button, it can now be selected:


![alt_text](https://drive.google.com/uc?id=19hCWdcmHU1mXS1dDUK7lKNl1RKh8H_BR "press_the_green_button_of_the_right_bottom_corner_to_refresh_licenses")

**TESTING:**

Let’s explain how the scene _Nice1CharacterSelectorSample_ works.

We must perform the login setup from the _Walletmanager _of the scene:


![alt_text](https://drive.google.com/uc?id=1mD60WINXKZGnrBaewTI05yaAi2vHwaS- "Walletmanager")


These are the default values it comes with:


![alt_text](https://drive.google.com/uc?id=1ShnfqQIzW_Z-gQUue3NXPkK_ipHxHIpf "default_values")


For testing purposes, we will use the account “niceonetest1” associated with the Jungle 4 testnet network, and its data can be viewed through the following link:

[https://local.bloks.io/account/simpleassets?loadContract=true&tab=Tables&table=sassets&account=simpleassets&scope=niceonetest1&limit=100&nodeUrl=http%3A%2F%2Fjungle4.cryptolions.io&systemDomain=eosio&hyperionUrl=https%3A%2F%2Fjungle4history.cryptolions.io](https://local.bloks.io/account/simpleassets?loadContract=true&tab=Tables&table=sassets&account=simpleassets&scope=niceonetest1&limit=100&nodeUrl=http%3A%2F%2Fjungle4.cryptolions.io&systemDomain=eosio&hyperionUrl=https%3A%2F%2Fjungle4history.cryptolions.io)

We can use any account from the networks listed below:

 \


![alt_text](https://drive.google.com/uc?id=1a3HlEhAus11lmxsq7stpYKJEwtLBxG9g "networks_listed")


To start, after running the scene, we need to click the “Login” button:


![alt_text](https://drive.google.com/uc?id=1HAfsPX1M0ixD4nizB69P8gzDwhxqSbwJ "game_login_button")

The Vuplex Webview will appear for us to view Anchor:

![alt_text](https://drive.google.com/uc?id=1rOS-6fEFyMXr8rlpHWEats5QTaMjRmxH "game_anchor_button")

We click on the “ANCHOR” button, and then on “Launch Anchor”:


![alt_text](https://drive.google.com/uc?id=1ytTQt1UH6p7h5OJdxbvi5IndEOkagGCO "game_anchor_qr")

After that, we log in from Anchor and if we don’t have the license, the following notice will appear:


![alt_text](https://drive.google.com/uc?id=1OfhqcDgovQ7m3weJCIoCkChUfdyC998Z "game_no_license")


In contrast, if we have the license, the character selection panel will appear for us:


![alt_text](https://drive.google.com/uc?id=1IARwPX4tN_Ln6B9zF2fASAbEKorLSoGR "character_selection")


The character licenses are configured from the GameObject _CharacterSelector_ in the scene:

![alt_text](https://drive.google.com/uc?id=1LdrwhFX13B5T0N-F-7qQaBKrygd70HmO "GameObject_CharacterSelector")

Its default data is as follows:

![alt_text](https://drive.google.com/uc?id=1EpBP-Oay9GGS5yCzhKb53pq_VxrXU6gX "default_data_character_selector")

If we click on the green button on the panel in the scene, the licenses will be reloaded. So, for example, if we change the last license to free and update:


![alt_text](https://drive.google.com/uc?id=1fvZomA32b3csko0KlQFz8mLPs6874qQ6 "we_change_the_last_license_to_free_and_update")


Now, we will be able to select the “Capsule” character:

![alt_text](https://drive.google.com/uc?id=1Rqu1Vk8FleRT8lL1DYwCXkKSJ83q8SAi "we_will_be_able_to_select_the_Capsule_character")

