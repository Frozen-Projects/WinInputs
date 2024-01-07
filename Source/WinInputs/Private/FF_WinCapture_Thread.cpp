// Fill out your copyright notice in the Description page of Project Settings.

#include "FF_WinCapture_Thread.h"
#include "FF_WinCapture.h"

// UE Includes.
#include "GenericPlatform/GenericApplication.h"		// Get monitor infos to select.

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
	if (this->ParentActor->MonitorIndex < 0)
	{
		this->MonitorIndex = 0;
	}

	else
	{
		this->MonitorIndex = this->ParentActor->MonitorIndex;
	}
	
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
	FDisplayMetrics Display;
	FDisplayMetrics::RebuildDisplayMetrics(Display);

	CapturedDatas.ScreenStart.X = Display.MonitorInfo[MonitorIndex].WorkArea.Left;
	CapturedDatas.ScreenStart.Y = Display.MonitorInfo[MonitorIndex].WorkArea.Top;
	CapturedDatas.Resolution.X = Display.MonitorInfo[MonitorIndex].DisplayRect.Right - Display.MonitorInfo[MonitorIndex].DisplayRect.Left;
	CapturedDatas.Resolution.Y = Display.MonitorInfo[MonitorIndex].DisplayRect.Bottom - Display.MonitorInfo[MonitorIndex].DisplayRect.Top;
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

	BitBlt(DC_Destination, 0, 0, CapturedDatas.Resolution.X, CapturedDatas.Resolution.Y, DC_Source, CapturedDatas.ScreenStart.X, CapturedDatas.ScreenStart.Y, SRCCOPY);
	ReleaseDC(NULL, DC_Source);

	return true;
}