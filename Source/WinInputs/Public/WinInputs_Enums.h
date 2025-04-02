#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EKeyboardInputs : uint8
{
	None = 0			UMETA(DisplayName = "None"),			// Default-safe
	WinButton = 0x5B	UMETA(DisplayName = "Windows"),			// VK_LWIN
	LeftAlt = 0xA4		UMETA(DisplayName = "Left Alt"),		// VK_LMENU
	LeftControl = 0xA2	UMETA(DisplayName = "Left Control"),	// VK_LCONTROL
	LeftShift = 0xA0	UMETA(DisplayName = "Left Shift"),		// VK_LSHIFT
	Tab = 0x09			UMETA(DisplayName = "Tab"),				// VK_TAB
	Enter = 0x0D		UMETA(DisplayName = "Enter"),			// VK_RETURN
	Space = 0x20		UMETA(DisplayName = "Space"),			// VK_SPACE
	ESC = 0x1B			UMETA(DisplayName = "Escape"),			// VK_ESCAPE
	Delete = 0x2E		UMETA(DisplayName = "Delete"),			// VK_DELETE
	Backspace = 0x08	UMETA(DisplayName = "Backspace"),		// VK_BACK
	CapsLock = 0x14		UMETA(DisplayName = "Caps Lock"),		// VK_CAPITAL

	Zero_Key = 0x30		UMETA(DisplayName = "0"),
	One_Key = 0x31		UMETA(DisplayName = "1"),
	Two_Key = 0x32		UMETA(DisplayName = "2"),
	Three_Key = 0x33	UMETA(DisplayName = "3"),
	Four_Key = 0x34		UMETA(DisplayName = "4"),
	Five_Key = 0x35		UMETA(DisplayName = "5"),
	Six_Key = 0x36		UMETA(DisplayName = "6"),
	Seven_Key = 0x37	UMETA(DisplayName = "7"),
	Eight_Key = 0x38	UMETA(DisplayName = "8"),
	Nine_Key = 0x39		UMETA(DisplayName = "9"),

	A_Key = 0x41		UMETA(DisplayName = "A"),
	B_Key = 0x42		UMETA(DisplayName = "B"),
	C_Key = 0x43		UMETA(DisplayName = "C"),
	TR_C_Key = 0xC7		UMETA(DisplayName = "Ç"),
	D_Key = 0x44		UMETA(DisplayName = "D"),
	E_Key = 0x45		UMETA(DisplayName = "E"),
	F_Key = 0x46		UMETA(DisplayName = "F"),
	G_Key = 0x47		UMETA(DisplayName = "G"),
	TR_G_Key = 0xD0		UMETA(DisplayName = "Ğ"),
	H_Key = 0x48		UMETA(DisplayName = "H"),
	I_Key = 0x49		UMETA(DisplayName = "I"),
	TR_I_Key = 0xDD		UMETA(DisplayName = "İ"),
	J_Key = 0x4A		UMETA(DisplayName = "J"),
	K_Key = 0x4B		UMETA(DisplayName = "K"),
	L_Key = 0x4C		UMETA(DisplayName = "L"),
	M_Key = 0x4D		UMETA(DisplayName = "M"),
	N_Key = 0x4E		UMETA(DisplayName = "N"),
	O_Key = 0x4F		UMETA(DisplayName = "O"),
	TR_O_Key = 0xD6		UMETA(DisplayName = "Ö"),
	P_Key = 0x50		UMETA(DisplayName = "P"),
	Q_Key = 0x51		UMETA(DisplayName = "Q"),
	R_Key = 0x52		UMETA(DisplayName = "R"),
	S_Key = 0x53		UMETA(DisplayName = "S"),
	TR_S_Key = 0xDE		UMETA(DisplayName = "Ş"),
	T_Key = 0x54		UMETA(DisplayName = "T"),
	U_Key = 0x55		UMETA(DisplayName = "U"),
	TR_U_Key = 0xDC		UMETA(DisplayName = "Ü"),
	V_Key = 0x56		UMETA(DisplayName = "V"),
	W_Key = 0x57		UMETA(DisplayName = "W"),
	X_Key = 0x58		UMETA(DisplayName = "X"),
	Y_Key = 0x59		UMETA(DisplayName = "Y"),
	Z_Key = 0x5A		UMETA(DisplayName = "Z")
};
