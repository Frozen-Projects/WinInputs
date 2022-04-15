// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Inputs.h"
#include "WinInputsBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/


USTRUCT(BlueprintType)
struct FWinInfos
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Editanywhere)
		FString WinName;

	UPROPERTY(BlueprintReadWrite, Editanywhere)
		int32 WinPID;

	UPROPERTY(BlueprintReadWrite, Editanywhere)
		FVector2D WinPos;

	UPROPERTY(BlueprintReadWrite, Editanywhere)
		FVector2D WinSize;

};

UCLASS()
class UWinInputsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Press Keyboard Input", ToolTip = "Description.", Keywords = "press, keyboard, input"), Category = "Win Inputs|Keyboard")
	static bool PressKeyboardInput(bool ReleaseAfterPress, EKeyboardInputs KeyboardButtons);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Release Keyboard Input", ToolTip = "Description.", Keywords = "release, keyboard, input"), Category = "Win Inputs|Keyboard")
	static bool ReleaseKeyboardInput(EKeyboardInputs KeyboardButtons);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Send Keyboard Macro", ToolTip = "Description.", Keywords = "send, keyboard, input, macro"), Category = "Win Inputs|Keyboard")
	static bool SendKeyboardMacro(TArray<EKeyboardInputs> Array_Buttons);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Mouse Position", ToolTip = "Description.", Keywords = "set, mouse, position, location"), Category = "Win Inputs|Mouse")
	static void SetMousePosition(FVector2D CursorPosition, FVector2D WidgetSize, FVector2D& OutCursorPosition);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Press Left", ToolTip = "Description.", Keywords = "press, mouse, left"), Category = "Win Inputs|Mouse")
	static bool MousePressLeft();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Release Left", ToolTip = "Description.", Keywords = "release, mouse, left"), Category = "Win Inputs|Mouse")
	static bool MouseReleaseLeft();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Press Right", ToolTip = "Description.", Keywords = "press, mouse, right"), Category = "Win Inputs|Mouse")
	static bool MousePressRight();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Release Right", ToolTip = "Description.", Keywords = "release, mouse, right"), Category = "Win Inputs|Mouse")
	static bool MouseReleaseRight();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Press Middle", ToolTip = "Description.", Keywords = "press, mouse, middle"), Category = "Win Inputs|Mouse")
	static bool MousePressMiddle();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Release Middle", ToolTip = "Description.", Keywords = "release, mouse, middle"), Category = "Win Inputs|Mouse")
	static bool MouseReleaseMiddle();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Wheel Up", ToolTip = "Description.", Keywords = "wheel, mouse, up"), Category = "Win Inputs|Mouse")
	static bool MouseWheelUp();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Wheel Down", ToolTip = "Description.", Keywords = "wheel, mouse, down"), Category = "Win Inputs|Mouse")
	static bool MouseWheelDown();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Active Windows", ToolTip = "Description.", Keywords = "get, windows, active"), Category = "Win Inputs|Windows")
	static void GetActiveWindows(TArray<FWinInfos>& WindowsInfos);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Bring Window Front", ToolTip = "It brings external window to front.", Keywords = "bring, window, front"), Category = "Win Inputs|Windows")
	static bool BringWindowFront(const FString WindowString);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Open Pocess with BP", ToolTip = "Opens an existing process with FWindowsPlatformProcess::OpenProcess()", Keywords = "open, process, bp"), Category = "Win Inputs|Windows")
	static void OpenProcessBP(int32 ProcessID);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Current Process ID", ToolTip = "It uses FWindowsPlatformProcess::GetCurrentProcessId()", Keywords = "get, current, pid, process, id"), Category = "Win Inputs|Windows")
	static int32 GetCurrentPID();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Minimize Selected Window", ToolTip = "Description.", Keywords = "minimize, window, selected"), Category = "Win Inputs|Windows")
	static bool MinimizeSelectedWindow(const FString WindowString);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Is UE in Top", ToolTip = "Description.", Keywords = "is, ue, top"), Category = "Win Inputs|Windows")
	static bool IsUETop();

};
