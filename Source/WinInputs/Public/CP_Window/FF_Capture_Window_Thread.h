// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// UE Includes.
#include "HAL/Runnable.h"

// Forward Declaration.
class FRunnableThread;
class AFF_Capture_Window;

class FFF_Capture_Thread_Window : public FRunnable
{
	
public:	
	// Sets default values for this actor's properties
	FFF_Capture_Thread_Window(AFF_Capture_Window* In_Parent_Actor);

	// Destructor.
	virtual ~FFF_Capture_Thread_Window() override;

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Toggle(bool bIsPause);

private:

	bool bStartThread = false;
	FRunnableThread* RunnableThread = nullptr;

	// Comes from Actor Class (actually itself).
	AFF_Capture_Window* ParentActor = nullptr;

	// Comes from Actor Class.
	FString ThreadName;

	// Comes from Actor Class.
	float SleepTime = 0.f;

	// UPROPERTY from Actor Class.
	bool bShowCursor = false;

#ifdef _WIN64

	FString WindowName;
	
	HWND TargetWindow = nullptr;
	HDC DC_Source = NULL;
	HDC DC_Destination = NULL;
	
	RECT Rectangle_Last;
	RECT Rectangle_Current;

	HBITMAP CapturedBitmap = NULL;
	BITMAPFILEHEADER BitmapFileHeader;
	BITMAPINFO BitmapInfo;

#endif

	FCapturedData CapturedData;

	virtual bool Callback_Init_DC(FString& Error);
	virtual bool Callback_Init_Bitmap(FString& Error, bool bReInit);
	virtual void Callback_GDI_Release();
	virtual void Callback_GDI_Buffer();
	virtual bool Callback_Cursor_Draw();

	// Check if rectangle size changed. It also updates current rectangle of target window.
	virtual bool IsWindowSizeChanged();
};