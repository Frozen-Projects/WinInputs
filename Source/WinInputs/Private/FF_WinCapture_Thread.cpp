// Fill out your copyright notice in the Description page of Project Settings.

#include "FF_WinCapture_Thread.h"
#include "FF_WinCapture.h"

THIRD_PARTY_INCLUDES_START
#include "Windows/WindowsHWrapper.h"
#include "winuser.h"
THIRD_PARTY_INCLUDES_END

FFF_WinCapture_Thread::FFF_WinCapture_Thread(AFF_WinCapture* In_Parent_Actor)
{
	if (In_Parent_Actor)
	{
		this->ParentActor = In_Parent_Actor;
	}

	this->RunnableThread = FRunnableThread::Create(this, *this->ParentActor->ThreadName);
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
	this->bStartThread = true;
	return true;
}

uint32 FFF_WinCapture_Thread::Run()
{
	while (this->bStartThread)
	{
		FString Error;
		if (!this->ParentActor->Data_Queue.Enqueue(this->Callback_Buffer_GDI_2(Error)))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Error));
		}

		FPlatformProcess::Sleep(0.03334);
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

FCapturedWindowDatas FFF_WinCapture_Thread::Callback_Buffer_GDI_1(FString& Error)
{
	if (this->ParentActor->WindowName.IsEmpty())
	{
		Error = "Capture target name shouldn't be empty.";
		return FCapturedWindowDatas();
	}

	HWND TargetHandle = NULL;
	HDC DesktopDeviceContext = NULL;

	if (this->ParentActor->WindowName == "Desktop")
	{
		TargetHandle = GetDesktopWindow();
		DesktopDeviceContext = GetDC(NULL);
	}

	else
	{
		TargetHandle = FindWindowA(NULL, TCHAR_TO_UTF8(*this->ParentActor->WindowName));
		DesktopDeviceContext = GetWindowDC(TargetHandle);
	}

	HDC CaptureContext = CreateCompatibleDC(DesktopDeviceContext);

	RECT CaputedRectangle;
	GetWindowRect(TargetHandle, &CaputedRectangle);
	FVector2D CaptureSize = FVector2D(CaputedRectangle.right - CaputedRectangle.left, CaputedRectangle.bottom - CaputedRectangle.top);
	size_t BufferSize = CaptureSize.X * CaptureSize.Y * sizeof(FColor);

	HBITMAP CapturedBitmap = CreateCompatibleBitmap(CaptureContext, CaptureSize.X, CaptureSize.Y);
	SelectObject(CaptureContext, CapturedBitmap);

	PrintWindow(TargetHandle, CaptureContext, PW_CLIENTONLY);
	
	BITMAPINFOHEADER BitmapInfoHeader;
	memset(&BitmapInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	BitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapInfoHeader.biPlanes = 1;
	BitmapInfoHeader.biBitCount = 32;
	BitmapInfoHeader.biWidth = CaptureSize.X;
	BitmapInfoHeader.biHeight = -CaptureSize.Y;
	BitmapInfoHeader.biCompression = BI_RGB;
	BitmapInfoHeader.biSizeImage = BufferSize;

	BitBlt(CaptureContext, 0, 0, CaptureSize.X, CaptureSize.Y, DesktopDeviceContext, 0, 0, SRCCOPY);
	//ReleaseDC(NULL, DesktopDeviceContext);

	FCapturedWindowDatas CapturedDatas;
	CapturedDatas.Buffer = (uint8*)malloc(BufferSize);
	CapturedDatas.BufferSize = BufferSize;
	CapturedDatas.Resolution = CaptureSize;
	
	GetDIBits(CaptureContext, CapturedBitmap, 0, CaptureSize.Y, CapturedDatas.Buffer, (BITMAPINFO*)&BitmapInfoHeader, DIB_RGB_COLORS);

	//DeleteObject(CapturedBitmap);
	//DeleteDC(CaptureContext);

	return CapturedDatas;
}

FCapturedWindowDatas FFF_WinCapture_Thread::Callback_Buffer_GDI_2(FString& Error)
{
	if (this->ParentActor->WindowName.IsEmpty())
	{
		Error = "Capture target name shouldn't be empty.";
		return FCapturedWindowDatas();
	}

	HWND TargetHandle = NULL;
	HDC DesktopDeviceContext = NULL;

	if (this->ParentActor->WindowName == "Desktop")
	{
		TargetHandle = GetDesktopWindow();
		DesktopDeviceContext = GetDC(NULL);
	}

	else
	{
		TargetHandle = FindWindowA(NULL, TCHAR_TO_UTF8(*this->ParentActor->WindowName));
		DesktopDeviceContext = GetWindowDC(TargetHandle);
	}

	HDC CaptureContext = CreateCompatibleDC(DesktopDeviceContext);

	RECT CaputedRectangle;
	GetWindowRect(TargetHandle, &CaputedRectangle);
	FVector2D CaptureSize = FVector2D(CaputedRectangle.right - CaputedRectangle.left, CaputedRectangle.bottom - CaputedRectangle.top);
	size_t BufferSize = CaptureSize.X * CaptureSize.Y * sizeof(FColor);

	BITMAPINFO BitmapInfo;
	memset(&BitmapInfo, 0, sizeof(BITMAPINFO));
	BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapInfo.bmiHeader.biWidth = CaptureSize.X;
	BitmapInfo.bmiHeader.biHeight = -CaptureSize.Y;
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = GetDeviceCaps(DesktopDeviceContext, BITSPIXEL);
	BitmapInfo.bmiHeader.biCompression = BI_RGB;

	FCapturedWindowDatas CapturedDatas;
	CapturedDatas.BufferSize = BufferSize;
	CapturedDatas.Resolution = CaptureSize;
	CapturedDatas.Buffer = (uint8*)malloc(BufferSize);
	HBITMAP CapturedBitmap = CreateDIBSection(CaptureContext, &BitmapInfo, DIB_RGB_COLORS, (void**)&CapturedDatas.Buffer, 0, 0);

	// Select it into your DC
	SelectObject(CaptureContext, CapturedBitmap);

	// Copy image from screen
	BitBlt(CaptureContext, 0, 0, CaptureSize.X, CaptureSize.X, DesktopDeviceContext, 1, 1, SRCCOPY);

	return CapturedDatas;
}

FCapturedWindowDatas FFF_WinCapture_Thread::Callback_Buffer_DX(FString& Error)
{
	return FCapturedWindowDatas();
}
