// Copyright Epic Games, Inc. All Rights Reserved.

#include "WinInputsBPLibrary.h"
#include <iostream>
#include <string>
#include "Windows/WindowsHWrapper.h" 
#include <process.h>
#include "winuser.h"
#include "WinInputs.h"

UWinInputsBPLibrary::UWinInputsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool UWinInputsBPLibrary::PressKeyboardInput(bool ReleaseAfterPress, TEnumAsByte<KeyboardInputs> KeyboardButtons)
{
    int Button = 0;
    
    switch (KeyboardButtons)
    {
    case WinButton:
        Button = VK_LWIN;
        break;
        
    case LeftAlt:
        Button = VK_LMENU;
        break;

    case LeftControl:
        Button = VK_LCONTROL;
        break;

    case LeftShift:
        Button = VK_LSHIFT;
        break;

    case Tab:
        Button = VK_TAB;
        break;

    case Enter:
        Button = VK_RETURN;
        break;

    case Space:
        Button = VK_SPACE;
        break;

    case ESC:
        Button = VK_ESCAPE;
        break;
    
    case Delete:
        Button = VK_DELETE;
        break;

    case Backspace:
        Button = VK_BACK;
        break;

    case CapsLock:
        Button = VK_CAPITAL;
        break;

    case Zero_Key:
        Button = 0x30;
        break;

    case One_Key:
        Button = 0x31;
        break;

    case Two_Key:
        Button = 0x32;
        break;

    case Three_Key:
        Button = 0x33;
        break;

    case Four_Key:
        Button = 0x34;
        break;

    case Five_Key:
        Button = 0x35;
        break;

    case Six_Key:
        Button = 0x36;
        break;

    case Seven_Key:
        Button = 0x37;
        break;

    case Eight_Key:
        Button = 0x38;
        break;

    case Nine_Key:
        Button = 0x39;
        break;

    case A_Key:
        Button = 0x41;
        break;

    case B_Key:
        Button = 0x42;
        break;

    case C_Key:
        Button = 0x43;
        break;

    case TR_C_Key:
        Button = 0x220;
        break;

    case D_Key:
        Button = 0x44;
        break;

    case E_Key:
        Button = 0x45;
        break;

    case F_Key:
        Button = 0x46;
        break;

    case G_Key:
        Button = 0x47;
        break;

    case TR_G_Key:
        Button = 0x219;
        break;

    case H_Key:
        Button = 0x48;
        break;

    case I_Key:
        Button = 0x49;
        break;

    case TR_I_Key:
        Button = 0x222;
        break;

    case J_Key:
        Button = 0x4A;
        break;

    case K_Key:
        Button = 0x4B;
        break;

    case L_Key:
        Button = 0x4C;
        break;

    case M_Key:
        Button = 0x4D;
        break;

    case N_Key:
        Button = 0x4E;
        break;

    case O_Key:
        Button = 0x4F;
        break;

    case TR_O_Key:
        Button = 0x191;
        break;

    case P_Key:
        Button = 0x50;
        break;

    case Q_Key:
        Button = 0x51;
        break;

    case R_Key:
        Button = 0x52;
        break;

    case S_Key:
        Button = 0x53;
        break;

    case TR_S_Key:
        Button = 0x186;
        break;

    case T_Key:
        Button = 0x54;
        break;

    case U_Key:
        Button = 0x55;
        break;

    case TR_U_Key:
        Button = 0x221;
        break;

    case V_Key:
        Button = 0x56;
        break;

    case W_Key:
        Button = 0x57;
        break;

    case X_Key:
        Button = 0x58;
        break;

    case Y_Key:
        Button = 0x59;
        break;

    case Z_Key:
        Button = 0x5A;
        break;

    default:
        Button = 0;
        break;
    }
    
    if (ReleaseAfterPress == true)
    {
        INPUT inputs[2] = {};

        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = Button;

        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = Button;
        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

        UINT SentInput = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

        if (SentInput == ARRAYSIZE(inputs))
        {
            return true;
        }

        else
        {
            return false;
        }
    }

    else
    {
        INPUT inputs[1] = {};

        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = Button;

        UINT SentInput = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

        if (SentInput == ARRAYSIZE(inputs))
        {
            return true;
        }

        else
        {
            return false;
        }
    }
}

bool UWinInputsBPLibrary::ReleaseKeyboardInput(TEnumAsByte<KeyboardInputs> KeyboardButtons)
{
    int Button = 0;

    switch (KeyboardButtons)
    {
    case WinButton:
        Button = VK_LWIN;
        break;

    case LeftAlt:
        Button = VK_LMENU;
        break;

    case LeftControl:
        Button = VK_LCONTROL;
        break;

    case LeftShift:
        Button = VK_LSHIFT;
        break;

    case Tab:
        Button = VK_TAB;
        break;

    case Enter:
        Button = VK_RETURN;
        break;

    case Space:
        Button = VK_SPACE;
        break;

    case ESC:
        Button = VK_ESCAPE;
        break;

    case Delete:
        Button = VK_DELETE;
        break;

    case Backspace:
        Button = VK_BACK;
        break;

    case CapsLock:
        Button = VK_CAPITAL;
        break;

    case Zero_Key:
        Button = 0x30;
        break;

    case One_Key:
        Button = 0x31;
        break;

    case Two_Key:
        Button = 0x32;
        break;

    case Three_Key:
        Button = 0x33;
        break;

    case Four_Key:
        Button = 0x34;
        break;

    case Five_Key:
        Button = 0x35;
        break;

    case Six_Key:
        Button = 0x36;
        break;

    case Seven_Key:
        Button = 0x37;
        break;

    case Eight_Key:
        Button = 0x38;
        break;

    case Nine_Key:
        Button = 0x39;
        break;

    case A_Key:
        Button = 0x41;
        break;

    case B_Key:
        Button = 0x42;
        break;

    case C_Key:
        Button = 0x43;
        break;

    case TR_C_Key:
        Button = 0x220;
        break;

    case D_Key:
        Button = 0x44;
        break;

    case E_Key:
        Button = 0x45;
        break;

    case F_Key:
        Button = 0x46;
        break;

    case G_Key:
        Button = 0x47;
        break;

    case TR_G_Key:
        Button = 0x219;
        break;

    case H_Key:
        Button = 0x48;
        break;

    case I_Key:
        Button = 0x49;
        break;

    case TR_I_Key:
        Button = 0x222;
        break;

    case J_Key:
        Button = 0x4A;
        break;

    case K_Key:
        Button = 0x4B;
        break;

    case L_Key:
        Button = 0x4C;
        break;

    case M_Key:
        Button = 0x4D;
        break;

    case N_Key:
        Button = 0x4E;
        break;

    case O_Key:
        Button = 0x4F;
        break;

    case TR_O_Key:
        Button = 0x191;
        break;

    case P_Key:
        Button = 0x50;
        break;

    case Q_Key:
        Button = 0x51;
        break;

    case R_Key:
        Button = 0x52;
        break;

    case S_Key:
        Button = 0x53;
        break;

    case TR_S_Key:
        Button = 0x186;
        break;

    case T_Key:
        Button = 0x54;
        break;

    case U_Key:
        Button = 0x55;
        break;

    case TR_U_Key:
        Button = 0x221;
        break;

    case V_Key:
        Button = 0x56;
        break;

    case W_Key:
        Button = 0x57;
        break;

    case X_Key:
        Button = 0x58;
        break;

    case Y_Key:
        Button = 0x59;
        break;

    case Z_Key:
        Button = 0x5A;
        break;

    default:
        Button = 0;
        break;
    }

    INPUT inputs[1] = {};

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = Button;
    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT SentInput = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

    if (SentInput == ARRAYSIZE(inputs))
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool UWinInputsBPLibrary::SendKeyboardMacro(TArray<TEnumAsByte<KeyboardInputs>> Array_Buttons)
{
    TArray<bool> AnyError;
    for (int32 ButtonIndex = 0; ButtonIndex < Array_Buttons.Num(); ButtonIndex++)
    {
        AnyError.Add(UWinInputsBPLibrary::PressKeyboardInput(false, Array_Buttons[ButtonIndex]));
    }

    for (int32 ButtonIndex = 0; ButtonIndex < Array_Buttons.Num(); ButtonIndex++)
    {
        AnyError.Add(UWinInputsBPLibrary::ReleaseKeyboardInput(Array_Buttons[ButtonIndex]));
    }

    if (AnyError.Contains(false) == true)
    {
        return false;
    }

    else
    {
        return true;
    }
}

void UWinInputsBPLibrary::SetMousePosition(int32 In_Pos_X, int32 In_Pos_Y, int32 Widget_Size_X, int32 Widget_Size_Y, int32& Out_Pos_X, int32& Out_Pos_Y)
{
    int32 Desktop_Resolution_X = GetSystemMetrics(SM_CXSCREEN);
    int32 Desktop_Resolution_Y = GetSystemMetrics(SM_CYSCREEN);

    float DPI_X = Desktop_Resolution_X / Widget_Size_X;
    float DPI_Y = Desktop_Resolution_Y / Widget_Size_Y;

    Out_Pos_X = In_Pos_X * DPI_X;
    Out_Pos_Y = In_Pos_Y * DPI_Y;

    SetCursorPos(Out_Pos_X, Out_Pos_Y);
}

bool UWinInputsBPLibrary::MousePressLeft()
{
    INPUT inputs[1] = {};

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    UINT SentInput = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

    if (SentInput == ARRAYSIZE(inputs))
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool UWinInputsBPLibrary::MouseReleaseLeft()
{
    INPUT inputs[1] = {};

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    UINT SentInput = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

    if (SentInput == ARRAYSIZE(inputs))
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool UWinInputsBPLibrary::MousePressRight()
{
    INPUT inputs[1] = {};

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

    UINT SentInput = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

    if (SentInput == ARRAYSIZE(inputs))
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool UWinInputsBPLibrary::MouseReleaseRight()
{
    INPUT inputs[1] = {};

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

    UINT SentInput = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

    if (SentInput == ARRAYSIZE(inputs))
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool UWinInputsBPLibrary::MousePressMiddle()
{
    INPUT inputs[1] = {};

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;

    UINT SentInput = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

    if (SentInput == ARRAYSIZE(inputs))
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool UWinInputsBPLibrary::MouseReleaseMiddle()
{
    INPUT inputs[1] = {};

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;

    UINT SentInput = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

    if (SentInput == ARRAYSIZE(inputs))
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool UWinInputsBPLibrary::MouseWheelUp()
{
    INPUT inputs[1] = {};

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_WHEEL;
    inputs[0].mi.mouseData = 120;

    UINT SentInput = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

    if (SentInput == ARRAYSIZE(inputs))
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool UWinInputsBPLibrary::MouseWheelDown()
{
    INPUT inputs[1] = {};

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_WHEEL;
    inputs[0].mi.mouseData = -120;

    UINT SentInput = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

    if (SentInput == ARRAYSIZE(inputs))
    {
        return true;
    }

    else
    {
        return false;
    }
}

void UWinInputsBPLibrary::GetActiveWindows(TArray<FWinInfos>& WindowsInfos)
{
    for (HWND WindowHandle = GetTopWindow(NULL); WindowHandle != NULL; WindowHandle = GetNextWindow(WindowHandle, GW_HWNDNEXT))
    {
        // If process is only a service (not have a Windows) and there is no title, don't count it.
        int WindowTitleLenght = GetWindowTextLength(WindowHandle);
        if (WindowTitleLenght == 0 || !IsWindowVisible(WindowHandle))
            continue;

        char* EachWindowTitle = new char[WindowTitleLenght + 1];
        GetWindowTextA(WindowHandle, EachWindowTitle, WindowTitleLenght + 1);

        FWinInfos STR_WinInfos;
        DWORD ProcessID;
        const FString WindowString = EachWindowTitle;

        RECT WinPos;
        RECT WinSize;

        // Program Manager is standart Window Process we don't have to count it.
        if (WindowString != "Program Manager")
        {
            GetWindowThreadProcessId(WindowHandle, &ProcessID);

            GetWindowRect(WindowHandle, &WinPos);
            STR_WinInfos.WinPos.X = WinPos.left;
            STR_WinInfos.WinPos.Y = WinPos.top;

            GetWindowRect(WindowHandle, &WinSize);
            STR_WinInfos.WinSize.X = WinSize.right - WinSize.left;
            STR_WinInfos.WinSize.Y = WinSize.bottom - WinSize.top;

            STR_WinInfos.WinName = WindowString;
            STR_WinInfos.WinPID = ProcessID;
            
            WindowsInfos.Add(STR_WinInfos);
        }
    }
}

bool UWinInputsBPLibrary::BringWindowFront(const FString WindowString)
{
    const CHAR* WindowChar = TCHAR_TO_ANSI(*WindowString);
    HWND WindowHandle = FindWindowA(NULL, WindowChar);

    // If window is minimized, first restore it.
    if (IsIconic(WindowHandle))
    {
        ShowWindow(WindowHandle, SW_RESTORE);
        return BringWindowToTop(WindowHandle);
    }

    else
    {
        return BringWindowToTop(WindowHandle);
    }
}

bool UWinInputsBPLibrary::MinimizeSelectedWindow(const FString WindowString)
{
    const CHAR* WindowChar = TCHAR_TO_ANSI(*WindowString);
    HWND WindowHandle = FindWindowA(NULL, WindowChar);

    return CloseWindow(WindowHandle);
}