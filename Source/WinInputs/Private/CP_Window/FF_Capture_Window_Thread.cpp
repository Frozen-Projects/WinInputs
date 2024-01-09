// Fill out your copyright notice in the Description page of Project Settings.

#include "CP_Window/FF_Capture_Window_Thread.h"
#include "CP_Window/FF_Capture_Window.h"

FFF_Capture_Thread_Window::FFF_Capture_Thread_Window(AFF_Capture_Window* In_Parent_Actor)
{
	if (In_Parent_Actor)
	{
		this->ParentActor = In_Parent_Actor;
	}

	this->RunnableThread = FRunnableThread::Create(this, *this->ParentActor->ThreadName);
}

FFF_Capture_Thread_Window::~FFF_Capture_Thread_Window()
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

bool FFF_Capture_Thread_Window::Init()
{
#ifdef _WIN64
	FString Error;
	
	if (!this->Callback_Init_DC(Error))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Error));
		return false;
	}

	if (!this->Callback_Init_Bitmap(Error, false))
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

uint32 FFF_Capture_Thread_Window::Run()
{
#ifdef _WIN64
	while (this->bStartThread)
	{
		RECT CurrentRectangle;
		GetWindowRect(TargetWindow, &CurrentRectangle);

		if (!this->CompareRects(LastSize, CurrentRectangle))
		{
			FString Error;
			if (!this->Callback_Init_Bitmap(Error, true))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Error));
			}

			this->ParentActor->Data_Queue.Empty();
		}

		this->Callback_GDI_Buffer();
		if (!this->ParentActor->Data_Queue.Enqueue(CapturedData))
		{
			UE_LOG(LogTemp, Warning, TEXT("There is a problem to enqueue captured window datas."));
		}
	}

	return 0;
#else
	return 0;
#endif
}

void FFF_Capture_Thread_Window::Stop()
{
	this->bStartThread = false;

#ifdef _WIN64
	this->Callback_GDI_Release();
#endif
}

void FFF_Capture_Thread_Window::Toggle(bool bIsPause)
{
	if (this->RunnableThread)
	{
		this->RunnableThread->Suspend();
	}
}

bool FFF_Capture_Thread_Window::Callback_Init_DC(FString& Error)
{
#ifdef _WIN64
	this->WindowName = this->ParentActor->WindowName;

	if (this->WindowName.IsEmpty())
	{
		Error = "Window name shouldn't be emptry.";
		return false;
	}

	TargetWindow = FindWindowExA(NULL, NULL, NULL, TCHAR_TO_UTF8(*this->WindowName));

	DC_Source = GetDC(TargetWindow);
	DC_Destination = CreateCompatibleDC(DC_Source);

	return true;
#else
	return false;
#endif
}

bool FFF_Capture_Thread_Window::Callback_Init_Bitmap(FString& Error, bool bReInit)
{
#ifdef _WIN64
	if (bReInit && CapturedBitmap)
	{
		DeleteObject(CapturedBitmap);
	}

	GetWindowRect(TargetWindow, &LastSize);

	CapturedData.Resolution.X = LastSize.right - LastSize.left;
	CapturedData.Resolution.Y = LastSize.bottom - LastSize.top;
	CapturedData.BufferSize = CapturedData.Resolution.X * CapturedData.Resolution.Y * sizeof(FColor);

	// Only available for screen capture.
	CapturedData.ScreenStart.X = 0;
	CapturedData.ScreenStart.Y = 0;
	
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

	CapturedBitmap = CreateDIBSection(DC_Destination, &BitmapInfo, DIB_RGB_COLORS, (void**)&CapturedData.Buffer, NULL, NULL);
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

void FFF_Capture_Thread_Window::Callback_GDI_Release()
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

void FFF_Capture_Thread_Window::Callback_GDI_Buffer()
{
#ifdef _WIN64

	// We need to copy window buffer and get its position continuesly. So, we can use this thread.

	RECT PositionRect;
	GetWindowRect(TargetWindow, &PositionRect);

	CapturedData.WindowLocation.X = PositionRect.left;
	CapturedData.WindowLocation.Y = PositionRect.top;

	// Actual buffer functions.

	PrintWindow(TargetWindow, DC_Source, 2);	// We additionally need this function to view window.
	BitBlt(DC_Destination, 0, 0, CapturedData.Resolution.X, CapturedData.Resolution.Y, DC_Source, CapturedData.ScreenStart.X, CapturedData.ScreenStart.Y, SRCCOPY);
#endif
}

bool FFF_Capture_Thread_Window::CompareRects(RECT Old, RECT New)
{
#ifdef _WIN64
	FVector2D Size_Last;
	Size_Last.X = Old.right - Old.left;
	Size_Last.Y = Old.bottom - Old.top;

	FVector2D Size_Current;
	Size_Current.X = New.right - New.left;
	Size_Current.Y = New.bottom - New.top;

	if (Size_Last == Size_Current)
	{
		return true;
	}

	else
	{
		return false;
	}
#else
	return false;
#endif
}