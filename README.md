# WinInputs

- Press Keyboard Input
- Release Keyboard Input
- Send Keyboard Macro
--------------------------------------------------------------------------------------------
- Get Active Windows
- Bring Window Front
- Minimize Window
- Is UE Top
- Set Window Title (UE4 Native Function)
--------------------------------------------------------------------------------------------
- Set Mouse Position
- Mouse Press Left
- Mouse Release Left
- Mouse Press Right
- Mouse Release Right
- Mouse Press Middle
- Mouse Release Middle
- Mouse Wheel Up
- Mouse Wheel Down
--------------------------------------------------------------------------------------------
**USED DEPENDENCIES**
- iostream
- string
- Windows/WindowsHWrapper.h
- winuser.h

**NOTE:**
- An application can't steal another application's focus (There was some solutions but they deprecated with latest Windows 10 builds). So, if you change top window from UE4's combo box, you can not change from it again without returning to game. For this reason, we integrated "Is UE Top" function. ComboBox will become disabled after top windows changes and it will become enabled and refresh its contents when top windows is UE game.
- GetActiveWindows will also gets each windows' ProcessID.
- You can use them with our DeviceInfos plugin's Helper TaskKill and Terminal Async functions.
- But be careful ! It will create same effect with Task Manager -> End Task !

**DESKTOP CAPTURE:**
- This plugin only sends input to Windows operating system. It does not capture desktop or windows.
- We did not include it because there are alternatives. So, it will be unnecessary coding operation to create a function with Desktop Duplication API and return it to Media Texture.
	- **Screen Capture Media Source plugin from UE4 Marketplace**
		- https://www.unrealengine.com/marketplace/en-US/product/38b887ee46ef471ebdb93d8cfb2ad96a

	- **WindowCapture2D plugin from UE4 Marketplace or Github**
		- https://www.unrealengine.com/marketplace/en-US/product/windowcapture2d
		- https://github.com/ayumax/WindowCapture2D
		- If you select this, you need to calculate correct mouse position with target window's position and size. We exposed all informations from GetActiveWindows.

	- **NDI Screen Capture**
		- You can start it directly with game if you know FWindowsPlatformProcess::CreateProc function or if you have our DeviceInfos plugin.
		- But user should select all desktop from **Capture Settings -> Configure RoI** only once. NDI will record it as JSON (you can find it in "%localappdata%\NDI\Studio Monitor") and start itself with same configurations.

**SAMPLE USECASES:**
- Using 3D Editors inside UE4 VR
- Access web browser inside UE4 VR (to view something or allow action in blockchain wallet)
- Creating UE4 based remote desktop system (Just pass inputs with an online api)

**SPESIFIC DESCRIPTION FOR BLOCKCHAIN WALLETS:**
- This doesn't and won't get any information from blockchain wallets.
- This doesn't and won't send any information to blockchain wallets.
- If you use it with a capturing solution, it merely shows desktop and send inputs to "OS itself rather than only game" for beter user exprience (without take off VR headset for every authorization process).
- We are not a blockchain related company. For this reason, we don't have extensive know-how for wallet SDKs. But as we know, all major NFT related wallets work as browser extensions and there is no C++ SDK to communicate with them. So, implementing them will be hard.
- At this point, we suggest developers to create two layered login system.
	- First one is game related authentication and the second one is wallet authentication.
	- Game should call browser with a unique URL which specified to user for every process. (You can do it with Launch Browser or FWindowsPlatformProcess::CreateProc)
	- User can view, allow or disallow that process with one of desktop capturing methods and our input plugin.
	- If user allow that process, game related API will get that answer from web3 based API and send continue response to that user's game session.
- If you know how to integrate wallets other than Enjin or Defungify, you should not be here ! GO TO WORK !

**PURPOSE:**
- This plugin created for "our" startup's internal "Microsoft Windows VR Desktop" projects !
- So, we will update it with "our needs" and right now we have other project priorities.

**UPDATES:**
- There is no dependency which related with Unreal Engine. All of them come from Microsoft Native APIs. So it is fully supported with all minor and major versions. 
- We compiled it with UE4.27 if you need it for different versions, just create a C++ Function Library, copy all codes and includes, compile it.

**PLATFORM SUPPORT:**
- This plugin specifically created for Microsoft Windows VR Platforms. So we don't and won't support other platforms.
- We suggest Oculus Overlay and Android Deeplink plugins (there is one in UE4 Marketplace) to Oculus Quest 2.

**CONTRIBUTIONS:**
- If you know better approaches feel free to give pull requests. We will appreciate for your contributions.
