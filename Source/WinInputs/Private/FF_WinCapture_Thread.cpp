// Fill out your copyright notice in the Description page of Project Settings.

#include "FF_WinCapture_Thread.h"
#include "FF_WinCapture.h"

// UE Includes.
#include "ImageCore.h"

THIRD_PARTY_INCLUDES_START
#include "Windows/WindowsHWrapper.h"
#include "winuser.h"

#include <iostream>  
#include <algorithm>  
THIRD_PARTY_INCLUDES_END

FFF_WinCapture_Thread::FFF_WinCapture_Thread(AFF_WinCapture* In_Parent_Actor)
{
	if (In_Parent_Actor)
	{
		this->ParentActor = In_Parent_Actor;
	}

	if (!this->ParentActor->ThreadName.IsEmpty())
	{
		this->RunnableThread = FRunnableThread::Create(this, *this->ParentActor->ThreadName);
	}
}

FFF_WinCapture_Thread::~FFF_WinCapture_Thread()
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

bool FFF_WinCapture_Thread::Init()
{
	if (this->ParentActor->WindowName.IsEmpty())
	{
		return false;
	}

	this->WindowName = this->ParentActor->WindowName;
	this->bStartThread = true;
	return true;
}

uint32 FFF_WinCapture_Thread::Run()
{
	while (this->bStartThread)
	{
		FString Error;
		if (!this->Callback_Get_Window_Buffer(Error))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Error));
		}

		//FPlatformProcess::Sleep(0.016);
	}

	return 0;
}

void FFF_WinCapture_Thread::Stop()
{
	this->bStartThread = false;
}

void FFF_WinCapture_Thread::Toggle(bool bIsPause)
{
	if (this->RunnableThread)
	{
		this->RunnableThread->Suspend();
	}
}

bool FFF_WinCapture_Thread::Callback_Get_Window_Buffer(FString& Error)
{
	HDC Target_Dc = NULL;
	HWND Target_Handle = NULL;
	
	if (this->WindowName == "Desktop")
	{
		Target_Handle = GetDesktopWindow();
		Target_Dc = GetDC(NULL);
	}

	else
	{
		Target_Handle = FindWindowA(NULL, TCHAR_TO_UTF8(*this->WindowName));
		Target_Dc = GetDC(Target_Handle);
	}

	RECT TargetWindowRect;
	GetWindowRect(Target_Handle, &TargetWindowRect);
	FVector2D CaptureSize = FVector2D(TargetWindowRect.right - TargetWindowRect.left, TargetWindowRect.bottom - TargetWindowRect.top);
	
	// Create compatible device context which will store the copied image
	HDC Compatible_Dc = CreateCompatibleDC(Target_Dc);

	// Get screen properties
	int iBpi = GetDeviceCaps(Target_Dc, BITSPIXEL);
	size_t BufferSize = CaptureSize.X * CaptureSize.Y * 4;

	// Fill BITMAPINFO struct
	BITMAPINFO info;
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth = CaptureSize.X;
	info.bmiHeader.biHeight = CaptureSize.Y;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = iBpi;
	info.bmiHeader.biCompression = BI_RGB;

	// Create bitmap, getting pointer to raw data (this is the important part)

	FImageView CapturedImage;
	CapturedImage.Format = ERawImageFormat::BGRA8;
	CapturedImage.GammaSpace = EGammaSpace::Linear;
	CapturedImage.NumSlices = 1;
	CapturedImage.SizeX = CaptureSize.X;
	CapturedImage.SizeY = CaptureSize.Y;

	HBITMAP hBitmap = CreateDIBSection(Compatible_Dc, &info, DIB_RGB_COLORS, (void**)&CapturedImage.RawData, 0, 0);
	
	// Select it into your DC
	SelectObject(Compatible_Dc, hBitmap);

	// Copy image from screen
	BitBlt(Compatible_Dc, 0, 0, CaptureSize.X, CaptureSize.Y, Target_Dc, 1, 1, SRCCOPY);

	FCapturedWindowDatas CapturedWindowDatas;
	CapturedWindowDatas.Result = CapturedImage;
	CapturedWindowDatas.BufferSize = BufferSize;

	if (!this->ParentActor->Circ_Captured_Window.Enqueue(CapturedWindowDatas))
	{
		Error = "There is a problem to enqueue captured window datas.";
		return false;
	}

	return true;
}