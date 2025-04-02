// Copyright Epic Games, Inc. All Rights Reserved.

// UE Functions.
#include "WinInputsBPLibrary.h"
#include "WinInputs.h"

UWinInputsBPLibrary::UWinInputsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool UWinInputsBPLibrary::PressKeyboardInput(bool ReleaseAfterPress, EKeyboardInputs KeyboardButtons)
{
    const int Button = static_cast<int>(KeyboardButtons);

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
    const int Button = static_cast<int>(KeyboardButtons);;

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
    OutCursorPosition.X = CursorPosition.X / (WidgetSize.X / GetSystemMetrics(SM_CXSCREEN));
    OutCursorPosition.Y = CursorPosition.Y / (WidgetSize.Y / GetSystemMetrics(SM_CYSCREEN));
    
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
        {
			continue;
        }

        char* EachWindowTitle = new char[WindowTitleLenght + 1];
        GetWindowTextA(WindowHandle, EachWindowTitle, WindowTitleLenght + 1);

        FWinInfos STR_WinInfos;
        DWORD ProcessID;
        const FString WindowString = EachWindowTitle;

        RECT WinPos;
        RECT WinSize;

        // Program Manager is standard Window Process we don't have to count it.
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

void UWinInputsBPLibrary::OpenProcessBP(int32 ProcessID)
{
    FWindowsPlatformProcess::OpenProcess((uint32_t)ProcessID);
}

int32 UWinInputsBPLibrary::GetCurrentPID()
{
    return (int32_t)FWindowsPlatformProcess::GetCurrentProcessId();
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