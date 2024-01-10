// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// UE Includes.
#include "HAL/Runnable.h"

// Forward Declaration.
class FRunnableThread;
class AFF_Capture_Screen;

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

	FCapturedData CapturedData;

	virtual bool Callback_GDI_Init(FString& Error);
	virtual void Callback_GDI_Release();
	virtual void Callback_GDI_Buffer();
	virtual void Callback_Cursor_Draw();

};