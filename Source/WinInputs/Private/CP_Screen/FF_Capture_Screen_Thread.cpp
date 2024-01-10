// Fill out your copyright notice in the Description page of Project Settings.

#include "CP_Screen/FF_Capture_Screen_Thread.h"
#include "CP_Screen/FF_Capture_Screen.h"

FFF_Capture_Thread_Screen::FFF_Capture_Thread_Screen(AFF_Capture_Screen* In_Parent_Actor)
{
	if (In_Parent_Actor)
	{
		this->ParentActor = In_Parent_Actor;
	}

	this->SleepTime = this->ParentActor->Rate;
	this->ThreadName = this->ParentActor->ThreadName;
	this->RunnableThread = FRunnableThread::Create(this, *this->ThreadName);
}

FFF_Capture_Thread_Screen::~FFF_Capture_Thread_Screen()
{
	if (this->RunnableThread)
	{
		this->RunnableThread->Suspend(true);
		this->bStartThread = false;
		this->RunnableThread->Suspend(false);
		this->RunnableThread->Kill(true);
		this->RunnableThread->WaitForCompletion();
		delete this->RunnableThread;
	}
}

bool FFF_Capture_Thread_Screen::Init()
{
#ifdef _WIN64
	FString Error;
	if (!this->Callback_GDI_Init(Error))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Error));
		return false;
	}

	this->bStartThread = true;
	return true;
#else
	return false;
#endif
}

uint32 FFF_Capture_Thread_Screen::Run()
{
#ifdef _WIN64
	while (this->bStartThread)
	{
		this->Callback_GDI_Buffer();
	}

	return 0;
#else
	return 0;
#endif
}

void FFF_Capture_Thread_Screen::Stop()
{
	this->bStartThread = false;
#ifdef _WIN64
	this->Callback_GDI_Release();
#endif
}

void FFF_Capture_Thread_Screen::Toggle(bool bIsPause)
{
	if (this->RunnableThread)
	{
		this->RunnableThread->Suspend(bIsPause);
	}
}

bool FFF_Capture_Thread_Screen::Callback_GDI_Init(FString& Error)
{
#ifdef _WIN64
	this->TargetMonitorInfo = this->ParentActor->TargetMonitorInfo;
	this->bShowCursor = this->ParentActor->bShowCursor;

	CapturedData.Resolution.X = TargetMonitorInfo.DisplayRect.Right - TargetMonitorInfo.DisplayRect.Left;
	CapturedData.Resolution.Y = TargetMonitorInfo.DisplayRect.Bottom - TargetMonitorInfo.DisplayRect.Top;
	CapturedData.BufferSize = CapturedData.Resolution.X * CapturedData.Resolution.Y * sizeof(FColor);

	// Only available for screen capture.
	CapturedData.ScreenStart.X = TargetMonitorInfo.WorkArea.Left;
	CapturedData.ScreenStart.Y = TargetMonitorInfo.WorkArea.Top;

	DC_Source = GetDC(NULL);
	DC_Destination = CreateCompatibleDC(DC_Source);

	BITMAPINFO BitmapInfo;
	memset(&BitmapInfo, 0, sizeof(BITMAPINFO));
	BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;
	BitmapInfo.bmiHeader.biWidth = CapturedData.Resolution.X;

	// We need to flip up right.
	BitmapInfo.bmiHeader.biHeight = -CapturedData.Resolution.Y;
	
	// Unreal Engine use 32 Bit color because alpha.
	BitmapInfo.bmiHeader.biBitCount = 32;

	CapturedBitmap = CreateDIBSection(DC_Destination, &BitmapInfo, DIB_RGB_COLORS, (void**)&CapturedData, NULL, NULL);
	if (!CapturedBitmap)
	{
		DeleteDC(DC_Destination);
		Error = "Unable to create bitmap.";
		return false;
	}

	SelectObject(DC_Destination, CapturedBitmap);

	return true;
#else
	return false;
#endif
}

void FFF_Capture_Thread_Screen::Callback_GDI_Release()
{
#ifdef _WIN64
	if (DC_Destination)
	{
		ReleaseDC(NULL, DC_Destination);
		DeleteDC(DC_Destination);
	}

	if (DC_Source)
	{
		ReleaseDC(NULL, DC_Source);
		DeleteDC(DC_Source);
	}

	if (CapturedBitmap)
	{
		DeleteObject(CapturedBitmap);
	}
#endif
}

void FFF_Capture_Thread_Screen::Callback_GDI_Buffer()
{
#ifdef _WIN64
	
	if (bShowCursor)
	{
		this->Callback_Cursor_Draw();
	}

	FDisplayMetrics Display;
	FDisplayMetrics::RebuildDisplayMetrics(Display);
	
	TArray<FString> MonitorIDs;
	for (int32 Index_Monitor = 0; Index_Monitor < Display.MonitorInfo.Num(); Index_Monitor++)
	{
		MonitorIDs.Add(Display.MonitorInfo[Index_Monitor].ID);
	}

	if (MonitorIDs.Find(TargetMonitorInfo.ID) == INDEX_NONE)
	{
		std::call_once(this->Once_Flag, [this]()
			{
				AsyncTask(ENamedThreads::GameThread, [this]()
					{
						UE_LOG(LogTemp, Warning, TEXT("Target monitor disconnected."));
						this->ParentActor->Destroy();
					}
				);
			}
		);

		return;
	}

	BitBlt(DC_Destination, 0, 0, CapturedData.Resolution.X, CapturedData.Resolution.Y, DC_Source, CapturedData.ScreenStart.X, CapturedData.ScreenStart.Y, SRCCOPY);

	if (!this->ParentActor->Data_Queue.Enqueue(CapturedData))
	{
		UE_LOG(LogTemp, Warning, TEXT("There is a problem to enqueue captured screen data."));
		FPlatformProcess::Sleep(this->SleepTime);
	}

#endif
}

void FFF_Capture_Thread_Screen::Callback_Cursor_Draw()
{
#ifdef _WIN64
	CURSORINFO cursor_info;
	memset(&cursor_info, 0, sizeof(CURSORINFO));
	cursor_info.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&cursor_info);
	 
	if (cursor_info.flags == CURSOR_SHOWING)
	{
		ICONINFO icon_info;
		memset(&icon_info, 0, sizeof(ICONINFO));
		icon_info.fIcon = false;
		GetIconInfo(cursor_info.hCursor, &icon_info);
		
		const int x = (cursor_info.ptScreenPos.x - TargetMonitorInfo.DisplayRect.Left - TargetMonitorInfo.DisplayRect.Left - icon_info.xHotspot) + TargetMonitorInfo.DisplayRect.Left;
		const int y = (cursor_info.ptScreenPos.y - TargetMonitorInfo.DisplayRect.Top - TargetMonitorInfo.DisplayRect.Top - icon_info.yHotspot) + TargetMonitorInfo.DisplayRect.Top;

		BITMAP bmpCursor;
		memset(&bmpCursor, 0, sizeof(bmpCursor));
		GetObjectA(icon_info.hbmColor, sizeof(bmpCursor), &bmpCursor);
		
		DrawIconEx(DC_Destination, x, y, cursor_info.hCursor, bmpCursor.bmWidth, bmpCursor.bmHeight, 0, NULL, DI_NORMAL);
	}
#endif
}