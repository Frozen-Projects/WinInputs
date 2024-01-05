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
	HDC hScreenDC = GetDC(0);
	// Create compatible device context which will store the copied image
	HDC hMyDC = CreateCompatibleDC(hScreenDC);

	// Get screen properties
	int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	int iBpi = GetDeviceCaps(hScreenDC, BITSPIXEL);

	// Fill BITMAPINFO struct
	BITMAPINFO info;
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth = iScreenWidth;
	info.bmiHeader.biHeight = iScreenHeight;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = iBpi;
	info.bmiHeader.biCompression = BI_RGB;

	// Create bitmap, getting pointer to raw data (this is the important part)
	FCapturedWindowDatas CapturedWindowDatas;
	HBITMAP hBitmap = CreateDIBSection(hMyDC, &info, DIB_RGB_COLORS, (void**)&CapturedWindowDatas.Buffer, 0, 0);
	// Select it into your DC
	SelectObject(hMyDC, hBitmap);

	// Copy image from screen
	BitBlt(hMyDC, 0, 0, iScreenWidth, iScreenHeight, hScreenDC, 0, 0, SRCCOPY);

	CapturedWindowDatas.WindowSize = FVector2D(iScreenWidth, iScreenHeight);
	CapturedWindowDatas.BufferSize = iScreenWidth * iScreenHeight * 4;

	if (!this->ParentActor->Circ_Captured_Window.Enqueue(CapturedWindowDatas))
	{
		Error = "There is a problem to enqueue captured window datas.";
		return false;
	}

	return true;
}