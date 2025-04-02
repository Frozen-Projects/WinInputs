#pragma once

#include "HAL/Runnable.h"

#include "Engine/GameEngine.h"

THIRD_PARTY_INCLUDES_START
#ifdef _WIN64
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/WindowsHWrapper.h"
#include <WinUser.h>
#include "wingdi.h"
#include "dwmapi.h"
#include "Windows/HideWindowsPlatformTypes.h"
#endif

#include <mutex>
#include <iostream>
#include <string>
#include <process.h>
THIRD_PARTY_INCLUDES_END