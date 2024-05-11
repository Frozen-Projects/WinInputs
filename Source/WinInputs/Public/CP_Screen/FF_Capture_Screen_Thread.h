// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// UE Includes.
#include "HAL/Runnable.h"

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
THIRD_PARTY_INCLUDES_END

// Forward Declaration.
class FRunnableThread;
class AFF_Capture_Screen;

struct FCapturedDataScreen
{

public:

	uint8* Buffer = nullptr;
	size_t BufferSize = 0;
	
	FVector2D Resolution;
	FVector2D ScreenStart;

	bool IsDataValid()
	{
		if (Buffer && BufferSize > 0 && Resolution.X > 0 && Resolution.Y > 0)
		{
			return true;
		}

		else
		{
			return false;
		}
	}
};

class FFF_Capture_Thread_Screen : public FRunnable
{
	
public:	
	// Sets default values for this actor's properties
	FFF_Capture_Thread_Screen(AFF_Capture_Screen* In_Parent_Actor);

	// Destructor.
	virtual ~FFF_Capture_Thread_Screen() override;

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Toggle(bool bIsPause);

private:

	// We use this to call capturer destroy only once.
	std::once_flag Once_Flag;

	bool bStartThread = false;
	FRunnableThread* RunnableThread = nullptr;

	// Comes from Actor Class (actually itself).
	AFF_Capture_Screen* ParentActor = nullptr;

	// Comes from Actor Class.
	FMonitorInfo TargetMonitorInfo;

	// Comes from Actor Class.
	FString ThreadName;

	// Comes from Actor Class.
	float SleepTime = 0.f;

	// UPROPERTY from Actor Class.
	bool bShowCursor = false;

#ifdef _WIN64

	HDC DC_Source = NULL;
	HDC DC_Destination = NULL;
	HBITMAP CapturedBitmap = NULL;

#endif

	FCapturedDataScreen CapturedData;

	virtual bool Callback_GDI_Init(FString& Error);
	virtual void Callback_GDI_Release();
	virtual void Callback_GDI_Buffer();
	virtual void Callback_Cursor_Draw();

};