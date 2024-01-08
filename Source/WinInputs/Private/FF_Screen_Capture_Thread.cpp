// Fill out your copyright notice in the Description page of Project Settings.

#include "FF_Capture_Screen_Thread.h"
#include "FF_Capture_Screen.h"

FFF_Capture_Screen_Thread::FFF_Capture_Screen_Thread(AFF_Capture_Screen* In_Parent_Actor)
{
	if (In_Parent_Actor)
	{
		this->ParentActor = In_Parent_Actor;
	}

	this->RunnableThread = FRunnableThread::Create(this, *this->ParentActor->ThreadName);
}

FFF_Capture_Screen_Thread::~FFF_Capture_Screen_Thread()
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

bool FFF_Capture_Screen_Thread::Init()
{
	this->TargetMonitorInfo = this->ParentActor->TargetMonitorInfo;
	this->bShowCursor = this->ParentActor->bShowCursor;

	FString Error;
	if (!this->Callback_GDI_Init(Error))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Error));
		return false;
	}

	this->bStartThread = true;
	return true;
}

uint32 FFF_Capture_Screen_Thread::Run()
{
	while (this->bStartThread)
	{
		FString Error;
		if (!this->Callback_GDI_Buffer(Error))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Error));
		}

		if (!this->ParentActor->Data_Queue.Enqueue(CapturedDatas))
		{
			UE_LOG(LogTemp, Warning, TEXT("There is a problem to enqueue captured window datas."));
		}
	}

	return 0;
}

void FFF_Capture_Screen_Thread::Stop()
{
	this->bStartThread = false;
	this->Callback_GDI_Release();
}

void FFF_Capture_Screen_Thread::Toggle(bool bIsPause)
{
	if (this->RunnableThread)
	{
		this->RunnableThread->Suspend();
	}
}

bool FFF_Capture_Screen_Thread::Callback_GDI_Init(FString& Error)
{
	CapturedDatas.ScreenStart.X = TargetMonitorInfo.WorkArea.Left;
	CapturedDatas.ScreenStart.Y = TargetMonitorInfo.WorkArea.Top;
	CapturedDatas.Resolution.X = TargetMonitorInfo.DisplayRect.Right - TargetMonitorInfo.DisplayRect.Left;
	CapturedDatas.Resolution.Y = TargetMonitorInfo.DisplayRect.Bottom - TargetMonitorInfo.DisplayRect.Top;
	CapturedDatas.BufferSize = CapturedDatas.Resolution.X * CapturedDatas.Resolution.Y * sizeof(FColor);

	DC_Destination = CreateCompatibleDC(NULL);

	BITMAPINFO BitmapInfo;
	memset(&BitmapInfo, 0, sizeof(BITMAPINFO));
	BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapInfo.bmiHeader.biWidth = CapturedDatas.Resolution.X;
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;

	// We need to flip up right.
	BitmapInfo.bmiHeader.biHeight = -CapturedDatas.Resolution.Y;
	
	// Unreal Engine use 32 Bit color because alpha.
	BitmapInfo.bmiHeader.biBitCount = 32;

	CapturedBitmap = CreateDIBSection(DC_Destination, &BitmapInfo, DIB_RGB_COLORS, (void**)&CapturedDatas.Buffer, NULL, NULL);
	if (!CapturedBitmap)
	{
		DeleteDC(DC_Destination);
		Error = "Unable to create bitmap.";
		return false;
	}

	SelectObject(DC_Destination, CapturedBitmap);

	return true;
}

void FFF_Capture_Screen_Thread::Callback_GDI_Release()
{
	if (DC_Destination)
	{
		ReleaseDC(NULL, DC_Destination);
		DeleteDC(DC_Destination);
	}

	if (CapturedBitmap)
	{
		DeleteObject(CapturedBitmap);
	}
}

bool FFF_Capture_Screen_Thread::Callback_GDI_Buffer(FString& Error)
{
	HDC DC_Source = GetDC(NULL);
	if (!DC_Source)
	{
		Error = "Unable to get source DC";
		return false;
	}

	if (bShowCursor)
	{
		this->Callback_Cursor_Draw();
	}

	BitBlt(DC_Destination, 0, 0, CapturedDatas.Resolution.X, CapturedDatas.Resolution.Y, DC_Source, CapturedDatas.ScreenStart.X, CapturedDatas.ScreenStart.Y, SRCCOPY);
	ReleaseDC(NULL, DC_Source);

	return true;
}

void FFF_Capture_Screen_Thread::Callback_Cursor_Draw()
{
	CURSORINFO cursor_info;
	memset(&cursor_info, 0, sizeof(CURSORINFO));
	cursor_info.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&cursor_info);
	 
	if (cursor_info.flags == CURSOR_SHOWING)
	{
		ICONINFO icon_info;
		memset(&icon_info, 0, sizeof(ICONINFO));
		GetIconInfo(cursor_info.hCursor, &icon_info);
		
		const int x = (cursor_info.ptScreenPos.x - TargetMonitorInfo.DisplayRect.Left - TargetMonitorInfo.DisplayRect.Left - icon_info.xHotspot) + TargetMonitorInfo.DisplayRect.Left;
		const int y = (cursor_info.ptScreenPos.y - TargetMonitorInfo.DisplayRect.Top - TargetMonitorInfo.DisplayRect.Top - icon_info.yHotspot) + TargetMonitorInfo.DisplayRect.Top;

		BITMAP bmpCursor;
		memset(&bmpCursor, 0, sizeof(bmpCursor));
		GetObjectA(icon_info.hbmColor, sizeof(bmpCursor), &bmpCursor);
		
		DrawIconEx(DC_Destination, x, y, cursor_info.hCursor, bmpCursor.bmWidth, bmpCursor.bmHeight, 0, NULL, DI_NORMAL);
	}
}