// Copyright Epic Games, Inc. All Rights Reserved.

// UE Functions.
#include "WinInputsBPLibrary.h"
#include "WinInputs.h"
#include "Engine/GameEngine.h"

// Custom Includes.
#include "Inputs.h"

// C++ Functions.
#include <iostream>
#include <string>
#include <process.h>

// Windows Functions.
#include "Windows/WindowsHWrapper.h"
#include "winuser.h"

UWinInputsBPLibrary::UWinInputsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool UWinInputsBPLibrary::PressKeyboardInput(bool ReleaseAfterPress, EKeyboardInputs KeyboardButtons)
{
    int Button = 0;
    switch (KeyboardButtons)
    {
    case EKeyboardInputs::WinButton:
        Button = VK_LWIN;
        break;
        
    case EKeyboardInputs::LeftAlt:
        Button = VK_LMENU;
        break;

    case EKeyboardInputs::LeftControl:
        Button = VK_LCONTROL;
        break;

    case EKeyboardInputs::LeftShift:
        Button = VK_LSHIFT;
        break;

    case EKeyboardInputs::Tab:
        Button = VK_TAB;
        break;

    case EKeyboardInputs::Enter:
        Button = VK_RETURN;
        break;

    case EKeyboardInputs::Space:
        Button = VK_SPACE;
        break;

    case EKeyboardInputs::ESC:
        Button = VK_ESCAPE;
        break;
    
    case EKeyboardInputs::Delete:
        Button = VK_DELETE;
        break;

    case EKeyboardInputs::Backspace:
        Button = VK_BACK;
        break;

    case EKeyboardInputs::CapsLock:
        Button = VK_CAPITAL;
        break;

    case EKeyboardInputs::Zero_Key:
        Button = 0x30;
        break;

    case EKeyboardInputs::One_Key:
        Button = 0x31;
        break;

    case EKeyboardInputs::Two_Key:
        Button = 0x32;
        break;

    case EKeyboardInputs::Three_Key:
        Button = 0x33;
        break;

    case EKeyboardInputs::Four_Key:
        Button = 0x34;
        break;

    case EKeyboardInputs::Five_Key:
        Button = 0x35;
        break;

    case EKeyboardInputs::Six_Key:
        Button = 0x36;
        break;

    case EKeyboardInputs::Seven_Key:
        Button = 0x37;
        break;

    case EKeyboardInputs::Eight_Key:
        Button = 0x38;
        break;

    case EKeyboardInputs::Nine_Key:
        Button = 0x39;
        break;

    case EKeyboardInputs::A_Key:
        Button = 0x41;
        break;

    case EKeyboardInputs::B_Key:
        Button = 0x42;
        break;

    case EKeyboardInputs::C_Key:
        Button = 0x43;
        break;

    case EKeyboardInputs::TR_C_Key:
        Button = 0x220;
        break;

    case EKeyboardInputs::D_Key:
        Button = 0x44;
        break;

    case EKeyboardInputs::E_Key:
        Button = 0x45;
        break;

    case EKeyboardInputs::F_Key:
        Button = 0x46;
        break;

    case EKeyboardInputs::G_Key:
        Button = 0x47;
        break;

    case EKeyboardInputs::TR_G_Key:
        Button = 0x219;
        break;

    case EKeyboardInputs::H_Key:
        Button = 0x48;
        break;

    case EKeyboardInputs::I_Key:
        Button = 0x49;
        break;

    case EKeyboardInputs::TR_I_Key:
        Button = 0x222;
        break;

    case EKeyboardInputs::J_Key:
        Button = 0x4A;
        break;

    case EKeyboardInputs::K_Key:
        Button = 0x4B;
        break;

    case EKeyboardInputs::L_Key:
        Button = 0x4C;
        break;

    case EKeyboardInputs::M_Key:
        Button = 0x4D;
        break;

    case EKeyboardInputs::N_Key:
        Button = 0x4E;
        break;

    case EKeyboardInputs::O_Key:
        Button = 0x4F;
        break;

    case EKeyboardInputs::TR_O_Key:
        Button = 0x191;
        break;

    case EKeyboardInputs::P_Key:
        Button = 0x50;
        break;

    case EKeyboardInputs::Q_Key:
        Button = 0x51;
        break;

    case EKeyboardInputs::R_Key:
        Button = 0x52;
        break;

    case EKeyboardInputs::S_Key:
        Button = 0x53;
        break;

    case EKeyboardInputs::TR_S_Key:
        Button = 0x186;
        break;

    case EKeyboardInputs::T_Key:
        Button = 0x54;
        break;

    case EKeyboardInputs::U_Key:
        Button = 0x55;
        break;

    case EKeyboardInputs::TR_U_Key:
        Button = 0x221;
        break;

    case EKeyboardInputs::V_Key:
        Button = 0x56;
        break;

    case EKeyboardInputs::W_Key:
        Button = 0x57;
        break;

    case EKeyboardInputs::X_Key:
        Button = 0x58;
        break;

    case EKeyboardInputs::Y_Key:
        Button = 0x59;
        break;

    case EKeyboardInputs::Z_Key:
        Button = 0x5A;
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

bool UWinInputsBPLibrary::ReleaseKeyboardInput(EKeyboardInputs KeyboardButtons)
{
    int Button = 0;
    switch (KeyboardButtons)
    {
    case EKeyboardInputs::WinButton:
        Button = VK_LWIN;
        break;

    case EKeyboardInputs::LeftAlt:
        Button = VK_LMENU;
        break;

    case EKeyboardInputs::LeftControl:
        Button = VK_LCONTROL;
        break;

    case EKeyboardInputs::LeftShift:
        Button = VK_LSHIFT;
        break;

    case EKeyboardInputs::Tab:
        Button = VK_TAB;
        break;

    case EKeyboardInputs::Enter:
        Button = VK_RETURN;
        break;

    case EKeyboardInputs::Space:
        Button = VK_SPACE;
        break;

    case EKeyboardInputs::ESC:
        Button = VK_ESCAPE;
        break;

    case EKeyboardInputs::Delete:
        Button = VK_DELETE;
        break;

    case EKeyboardInputs::Backspace:
        Button = VK_BACK;
        break;

    case EKeyboardInputs::CapsLock:
        Button = VK_CAPITAL;
        break;

    case EKeyboardInputs::Zero_Key:
        Button = 0x30;
        break;

    case EKeyboardInputs::One_Key:
        Button = 0x31;
        break;

    case EKeyboardInputs::Two_Key:
        Button = 0x32;
        break;

    case EKeyboardInputs::Three_Key:
        Button = 0x33;
        break;

    case EKeyboardInputs::Four_Key:
        Button = 0x34;
        break;

    case EKeyboardInputs::Five_Key:
        Button = 0x35;
        break;

    case EKeyboardInputs::Six_Key:
        Button = 0x36;
        break;

    case EKeyboardInputs::Seven_Key:
        Button = 0x37;
        break;

    case EKeyboardInputs::Eight_Key:
        Button = 0x38;
        break;

    case EKeyboardInputs::Nine_Key:
        Button = 0x39;
        break;

    case EKeyboardInputs::A_Key:
        Button = 0x41;
        break;

    case EKeyboardInputs::B_Key:
        Button = 0x42;
        break;

    case EKeyboardInputs::C_Key:
        Button = 0x43;
        break;

    case EKeyboardInputs::TR_C_Key:
        Button = 0x220;
        break;

    case EKeyboardInputs::D_Key:
        Button = 0x44;
        break;

    case EKeyboardInputs::E_Key:
        Button = 0x45;
        break;

    case EKeyboardInputs::F_Key:
        Button = 0x46;
        break;

    case EKeyboardInputs::G_Key:
        Button = 0x47;
        break;

    case EKeyboardInputs::TR_G_Key:
        Button = 0x219;
        break;

    case EKeyboardInputs::H_Key:
        Button = 0x48;
        break;

    case EKeyboardInputs::I_Key:
        Button = 0x49;
        break;

    case EKeyboardInputs::TR_I_Key:
        Button = 0x222;
        break;

    case EKeyboardInputs::J_Key:
        Button = 0x4A;
        break;

    case EKeyboardInputs::K_Key:
        Button = 0x4B;
        break;

    case EKeyboardInputs::L_Key:
        Button = 0x4C;
        break;

    case EKeyboardInputs::M_Key:
        Button = 0x4D;
        break;

    case EKeyboardInputs::N_Key:
        Button = 0x4E;
        break;

    case EKeyboardInputs::O_Key:
        Button = 0x4F;
        break;

    case EKeyboardInputs::TR_O_Key:
        Button = 0x191;
        break;

    case EKeyboardInputs::P_Key:
        Button = 0x50;
        break;

    case EKeyboardInputs::Q_Key:
        Button = 0x51;
        break;

    case EKeyboardInputs::R_Key:
        Button = 0x52;
        break;

    case EKeyboardInputs::S_Key:
        Button = 0x53;
        break;

    case EKeyboardInputs::TR_S_Key:
        Button = 0x186;
        break;

    case EKeyboardInputs::T_Key:
        Button = 0x54;
        break;

    case EKeyboardInputs::U_Key:
        Button = 0x55;
        break;

    case EKeyboardInputs::TR_U_Key:
        Button = 0x221;
        break;

    case EKeyboardInputs::V_Key:
        Button = 0x56;
        break;

    case EKeyboardInputs::W_Key:
        Button = 0x57;
        break;

    case EKeyboardInputs::X_Key:
        Button = 0x58;
        break;

    case EKeyboardInputs::Y_Key:
        Button = 0x59;
        break;

    case EKeyboardInputs::Z_Key:
        Button = 0x5A;
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

bool UWinInputsBPLibrary::SendKeyboardMacro(TArray<EKeyboardInputs> Array_Buttons)
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

void UWinInputsBPLibrary::SetMousePosition(FVector2D CursorPosition, FVector2D WidgetSize, FVector2D& OutCursorPosition)
{
    int32 Desktop_Resolution_X = GetSystemMetrics(SM_CXSCREEN);
    int32 Desktop_Resolution_Y = GetSystemMetrics(SM_CYSCREEN);

    float DPI_X = Desktop_Resolution_X / WidgetSize.X;
    float DPI_Y = Desktop_Resolution_Y / WidgetSize.Y;

    OutCursorPosition.X = CursorPosition.X * DPI_X;
    OutCursorPosition.Y = CursorPosition.Y * DPI_Y;
    
    SetCursorPos(int(FMath::TruncToInt(OutCursorPosition.X)), int(FMath::TruncToInt(OutCursorPosition.Y)));
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

            STR_WinInfos.WinName = EachWindowTitle;
            STR_WinInfos.WinPID = ProcessID;
            
            WindowsInfos.Add(STR_WinInfos);
        }
    }
}

bool UWinInputsBPLibrary::BringWindowFront(const FString WindowString)
{
    HWND WindowHandle = FindWindowA(NULL, TCHAR_TO_ANSI(*WindowString));

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
    HWND WindowHandle = FindWindowA(NULL, TCHAR_TO_ANSI(*WindowString));

    return CloseWindow(WindowHandle);
}

bool UWinInputsBPLibrary::IsUETop()
{
    UGameEngine* GameEngine = Cast<UGameEngine>(GEngine);
    if (GameEngine != nullptr)
    {
        TSharedPtr<SWindow> GameViewportWindow = GameEngine->GameViewportWindow.Pin();
        return GameViewportWindow->GetNativeWindow()->IsForegroundWindow();
    }

    else
    {
        return false;
    }
}