// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// UE Includes.
#include "HAL/Runnable.h"

// Fordward Declerations.
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

protected:
	
	bool bStartThread = false;
	FRunnableThread* RunnableThread = nullptr;
	AFF_Capture_Screen* ParentActor = nullptr;

private:

	// UPROPERTY from Actor Class.
	bool bShowCursor = false;

	// Comes from Actor Class.
	FMonitorInfo TargetMonitorInfo;

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