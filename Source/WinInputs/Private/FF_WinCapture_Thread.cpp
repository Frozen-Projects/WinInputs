// Fill out your copyright notice in the Description page of Project Settings.

#include "FF_WinCapture_Thread.h"
#include "FF_WinCapture.h"

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
	FString Error;
	if (!this->Callback_GDI_Init(Error))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Error));
		return false;
	}

	this->bStartThread = true;
	return true;
}

uint32 FFF_WinCapture_Thread::Run()
{
	while (this->bStartThread)
	{
		FString Error;
		if (!this->Callback_GDI_Buffer(Error) || !this->ParentActor->Data_Queue.Enqueue(CapturedDatas))
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
	this->Callback_GDI_Release();
}

void FFF_WinCapture_Thread::Toggle(bool bIsPause)
{
	if (this->RunnableThread)
	{
		this->RunnableThread->Suspend();
	}
}

bool FFF_WinCapture_Thread::Callback_GDI_Init(FString& Error)
{
	DesktopHandle = GetDesktopWindow();
	DC_Destination = CreateCompatibleDC(NULL);

	RECT CaputedRectangle;
	GetWindowRect(DesktopHandle, &CaputedRectangle);
	CapturedDatas.Resolution = FVector2D(CaputedRectangle.right - CaputedRectangle.left, CaputedRectangle.bottom - CaputedRectangle.top);
	CapturedDatas.BufferSize = CapturedDatas.Resolution.X * CapturedDatas.Resolution.Y * sizeof(FColor);

	BITMAPINFO BitmapInfo;
	memset(&BitmapInfo, 0, sizeof(BITMAPINFO));
	BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapInfo.bmiHeader.biWidth = CapturedDatas.Resolution.X;
	
	// We need to flip up right.
	BitmapInfo.bmiHeader.biHeight = -CapturedDatas.Resolution.Y;
	
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;

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

void FFF_WinCapture_Thread::Callback_GDI_Release()
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

bool FFF_WinCapture_Thread::Callback_GDI_Buffer(FString& Error)
{
	HDC DC_Source = GetDC(NULL);
	if (!DC_Source)
	{
		Error = "Unable to get source DC";
		return false;
	}

	BitBlt(DC_Destination, 0, 0, CapturedDatas.Resolution.X, CapturedDatas.Resolution.Y, DC_Source, 1, 1, SRCCOPY);
	ReleaseDC(NULL, DC_Source);

	return true;
}