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
#include "Windows/HideWindowsPlatformTypes.h"
#endif
THIRD_PARTY_INCLUDES_END

// Fordward Declerations.
class FRunnableThread;
class AFF_WinCapture;

struct FCapturedWindowDatas
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

class FFF_WinCapture_Thread : public FRunnable
{
	
public:	
	// Sets default values for this actor's properties
	FFF_WinCapture_Thread(AFF_WinCapture* In_Parent_Actor);

	// Destructor.
	virtual ~FFF_WinCapture_Thread() override;

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Toggle(bool bIsPause);

protected:
	
	bool bStartThread = false;
	FRunnableThread* RunnableThread = nullptr;
	AFF_WinCapture* ParentActor = nullptr;

private:

	HDC DC_Destination = NULL;
	HBITMAP CapturedBitmap = NULL;
	int32 MonitorIndex = 0;

	FCapturedWindowDatas CapturedDatas;

	virtual bool Callback_GDI_Init(FString& Error);
	virtual void Callback_GDI_Release();
	virtual bool Callback_GDI_Buffer(FString& Error);

};