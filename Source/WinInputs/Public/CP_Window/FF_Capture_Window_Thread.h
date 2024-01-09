// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// UE Includes.
#include "HAL/Runnable.h"

// Fordward Declerations.
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

protected:
	
	bool bStartThread = false;
	FRunnableThread* RunnableThread = nullptr;
	AFF_Capture_Window* ParentActor = nullptr;

private:

	FString ThreadName;

#ifdef _WIN64

	FString WindowName;
	
	HWND TargetWindow = nullptr;
	
	RECT Rectangle_Last;
	RECT Rectangle_Current;

	HDC DC_Source = NULL;
	HDC DC_Destination = NULL;
	HBITMAP CapturedBitmap = NULL;

#endif

	// UPROPERTY from Actor Class.
	bool bShowCursor = false;

	FCapturedData CapturedData;

	virtual bool Callback_Init_DC(FString& Error);
	virtual bool Callback_Init_Bitmap(FString& Error, bool bReInit);

	virtual void Callback_GDI_Release();
	virtual void Callback_GDI_Buffer();

	virtual void Callback_Cursor_Draw();

	// Check if rectangle size changed. It also updates current rectangle of target window.
	virtual bool IsWindowSizeChanged();

};