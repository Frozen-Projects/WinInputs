// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
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

UENUM()
enum KeyboardInputs
{
	WinButton		UMETA(DisplayName = "Windows"),
	LeftAlt			UMETA(DisplayName = "Left Alt"),
	LeftControl		UMETA(DisplayName = "Left Control"),
	LeftShift		UMETA(DisplayName = "Left Shift"),
	Tab				UMETA(DisplayName = "Tab"),
	Enter			UMETA(DisplayName = "Enter"),
	Space			UMETA(DisplayName = "Space"),
	ESC				UMETA(DisplayName = "Escape"),
	Delete			UMETA(DisplayName = "Delete"),
	Backspace		UMETA(DisplayName = "Backspace"),
	CapsLock		UMETA(DisplayName = "Caps Lock"),
	Zero_Key		UMETA(DisplayName = "0"),
	One_Key			UMETA(DisplayName = "1"),
	Two_Key			UMETA(DisplayName = "2"),
	Three_Key		UMETA(DisplayName = "3"),
	Four_Key		UMETA(DisplayName = "4"),
	Five_Key		UMETA(DisplayName = "5"),
	Six_Key			UMETA(DisplayName = "6"),
	Seven_Key		UMETA(DisplayName = "7"),
	Eight_Key		UMETA(DisplayName = "8"),
	Nine_Key		UMETA(DisplayName = "9"),
	A_Key			UMETA(DisplayName = "A"),
	B_Key			UMETA(DisplayName = "B"),
	C_Key			UMETA(DisplayName = "C"),
	TR_C_Key		UMETA(DisplayName = "Ç"),
	D_Key			UMETA(DisplayName = "D"),
	E_Key			UMETA(DisplayName = "E"),
	F_Key			UMETA(DisplayName = "F"),
	G_Key			UMETA(DisplayName = "G"),
	TR_G_Key		UMETA(DisplayName = "Ğ"),
	H_Key			UMETA(DisplayName = "H"),
	I_Key			UMETA(DisplayName = "I"),
	TR_I_Key		UMETA(DisplayName = "İ"),
	J_Key			UMETA(DisplayName = "J"),
	K_Key			UMETA(DisplayName = "K"),
	L_Key			UMETA(DisplayName = "L"),
	M_Key			UMETA(DisplayName = "M"),
	N_Key			UMETA(DisplayName = "N"),
	O_Key			UMETA(DisplayName = "O"),
	TR_O_Key		UMETA(DisplayName = "Ö"),
	P_Key			UMETA(DisplayName = "P"),
	Q_Key			UMETA(DisplayName = "Q"),
	R_Key			UMETA(DisplayName = "R"),
	S_Key			UMETA(DisplayName = "S"),
	TR_S_Key		UMETA(DisplayName = "Ş"),
	T_Key			UMETA(DisplayName = "T"),
	U_Key			UMETA(DisplayName = "U"),
	TR_U_Key		UMETA(DisplayName = "Ü"),
	V_Key			UMETA(DisplayName = "V"),
	W_Key			UMETA(DisplayName = "W"),
	X_Key			UMETA(DisplayName = "X"),
	Y_Key			UMETA(DisplayName = "Y"),
	Z_Key			UMETA(DisplayName = "Z"),
};

USTRUCT(BlueprintType)
struct FWinInfos
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Editanywhere)
		FString WinName;

	UPROPERTY(BlueprintReadWrite, Editanywhere)
		int32 WinPID;

};

UCLASS()
class UWinInputsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Press Keyboard Input", ToolTip = "Description.", Keywords = "press, keyboard, input"), Category = "Win Inputs")
	static bool PressKeyboardInput(bool ReleaseAfterPress, TEnumAsByte<KeyboardInputs> KeyboardButtons);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Release Keyboard Input", ToolTip = "Description.", Keywords = "release, keyboard, input"), Category = "Win Inputs")
	static bool ReleaseKeyboardInput(TEnumAsByte<KeyboardInputs> KeyboardButtons);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Send Keyboard Macro", ToolTip = "Description.", Keywords = "send, keyboard, input, macro"), Category = "Win Inputs")
	static bool SendKeyboardMacro(TArray<TEnumAsByte<KeyboardInputs>> Array_Buttons);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Active Windows", ToolTip = "Description.", Keywords = "get, active, windows"), Category = "Win Inputs")
	static void GetActiveWindows(TArray<FWinInfos>& WindowsInfos);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Bring Window Front", ToolTip = "Description.", Keywords = "bring, window, front"), Category = "Win Inputs")
	static bool BringWindowFront(const FString WindowString);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Mouse Position", ToolTip = "Description.", Keywords = "set, mouse, position, location"), Category = "Win Inputs")
	static void SetMousePosition(int32 In_Pos_X, int32 In_Pos_Y, int32 Widget_Size_X, int32 Widget_Size_Y, int32& Out_Pos_X, int32& Out_Pos_Y);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Press Left", ToolTip = "Description.", Keywords = "press, mouse, left"), Category = "Win Inputs")
	static bool MousePressLeft();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Release Left", ToolTip = "Description.", Keywords = "release, mouse, left"), Category = "Win Inputs")
	static bool MouseReleaseLeft();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Press Right", ToolTip = "Description.", Keywords = "press, mouse, right"), Category = "Win Inputs")
	static bool MousePressRight();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Release Right", ToolTip = "Description.", Keywords = "release, mouse, right"), Category = "Win Inputs")
	static bool MouseReleaseRight();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Press Middle", ToolTip = "Description.", Keywords = "press, mouse, middle"), Category = "Win Inputs")
	static bool MousePressMiddle();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Release Middle", ToolTip = "Description.", Keywords = "release, mouse, middle"), Category = "Win Inputs")
	static bool MouseReleaseMiddle();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Wheel Up", ToolTip = "Description.", Keywords = "wheel, mouse, up"), Category = "Win Inputs")
	static bool MouseWheelUp();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Mouse Wheel Down", ToolTip = "Description.", Keywords = "wheel, mouse, down"), Category = "Win Inputs")
	static bool MouseWheelDown();

};
