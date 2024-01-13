// Fill out your copyright notice in the Description page of Project Settings.

#include "CP_Window/FF_Capture_Window_Thread.h"
#include "CP_Window/FF_Capture_Window.h"

FFF_Capture_Thread_Window::FFF_Capture_Thread_Window(AFF_Capture_Window* In_Parent_Actor)
{
	if (In_Parent_Actor)
	{
		this->ParentActor = In_Parent_Actor;
	}

	this->SleepTime = this->ParentActor->Rate;
	this->ThreadName = this->ParentActor->ThreadName;
	this->RunnableThread = FRunnableThread::Create(this, *this->ThreadName);
}

FFF_Capture_Thread_Window::~FFF_Capture_Thread_Window()
{
	if (this->RunnableThread)
	{
		this->RunnableThread->Suspend(true);
		this->bStartThread = false;
		this->RunnableThread->Suspend(false);
		this->RunnableThread->WaitForCompletion();
		this->RunnableThread->Kill(true);
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
		if (!this->IsWindowSizeChanged())
		{
			FString Error;
			if (!this->Callback_Init_Bitmap(Error, true))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Error));
			}

			// If target window size changed, we need to clear data queue before continue.
			this->ParentActor->Data_Queue.Empty();
		}

		this->Callback_GDI_Buffer();
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
		this->RunnableThread->Suspend(bIsPause);
	}
}

bool FFF_Capture_Thread_Window::Callback_Init_DC(FString& Error)
{
#ifdef _WIN64
	this->WindowName = this->ParentActor->WindowName;

	if (this->WindowName.IsEmpty())
	{
		Error = "Window name shouldn't be empty.";
		return false;
	}

	TargetWindow = FindWindowExA(NULL, NULL, NULL, TCHAR_TO_UTF8(*this->WindowName));

	if (!TargetWindow)
	{
		Error = "Window handle is invalid.";
		return false;
	}

	DC_Source = GetDC(TargetWindow);
	DC_Destination = CreateCompatibleDC(DC_Source);

	this->bShowCursor = this->ParentActor->bShowCursor;

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

	GetWindowRect(TargetWindow, &Rectangle_Last);

	CapturedData.Resolution.X = Rectangle_Last.right - Rectangle_Last.left;
	CapturedData.Resolution.Y = Rectangle_Last.bottom - Rectangle_Last.top;
	CapturedData.BufferSize = CapturedData.Resolution.X * CapturedData.Resolution.Y * 4;
	CapturedData.Stride = (((32 * (size_t)CapturedData.Resolution.X + 31) & ~31) / 8.f) * CapturedData.Resolution.Y;
	CapturedData.Buffer = (uint8*)malloc(CapturedData.BufferSize);

	memset(&BitmapFileHeader, 0, sizeof(BITMAPFILEHEADER));
	BitmapFileHeader.bfType = 0x4D42;
	BitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	memset(&BitmapInfo, 0, sizeof(BITMAPINFO));
	BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;
	BitmapInfo.bmiHeader.biWidth = CapturedData.Resolution.X;

	// We need to flip up right.
	BitmapInfo.bmiHeader.biHeight = -CapturedData.Resolution.Y;
	
	// Unreal Engine use 32 Bit color because alpha.
	BitmapInfo.bmiHeader.biBitCount = 32;

	CapturedBitmap = CreateDIBSection(DC_Destination, &BitmapInfo, DIB_RGB_COLORS, (void**)&TempBuffer, NULL, NULL);
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

	CapturedData = FCapturedDataWindow();

#endif
}

void FFF_Capture_Thread_Window::Callback_GDI_Buffer()
{
#ifdef _WIN64

	RECT PositionRect;
	GetWindowRect(TargetWindow, &PositionRect);

	CapturedData.WindowLocation.X = PositionRect.left;
	CapturedData.WindowLocation.Y = PositionRect.top;

	if (this->ParentActor->bUseHaCompability)
	{
		PrintWindow(TargetWindow, DC_Source, 2);
	}

	if (!BitBlt(DC_Destination, 0, 0, CapturedData.Resolution.X, CapturedData.Resolution.Y, DC_Source, 0, 0, SRCCOPY))
	{
		return;
	}

	if (bShowCursor)
	{
		if (!this->Callback_Cursor_Draw())
		{
			return;
		}
	}

	if (!TempBuffer)
	{
		return;
	}

	if (this->ParentActor->bUseHaCompability)
	{
		CapturedData.Buffer = nullptr;
		CapturedData.Buffer = TempBuffer;
	}

	else
	{
		for (int32 Index_Pixel = 0; Index_Pixel < CapturedData.Resolution.X * CapturedData.Resolution.Y; Index_Pixel++)
		{
			int32 Index_Buffer = Index_Pixel * 4;
			CapturedData.Buffer[Index_Buffer] = TempBuffer[Index_Buffer];
			CapturedData.Buffer[Index_Buffer + 1] = TempBuffer[Index_Buffer + 1];
			CapturedData.Buffer[Index_Buffer + 2] = TempBuffer[Index_Buffer + 2];
			CapturedData.Buffer[Index_Buffer + 3] = 255;
		}
	}

	if (!this->ParentActor->Data_Queue.Enqueue(CapturedData))
	{
		UE_LOG(LogTemp, Warning, TEXT("There is a problem with enqueue."));
		FPlatformProcess::Sleep(this->SleepTime);

		if (this->ParentActor->Data_Queue.IsFull())
		{
			this->ParentActor->Data_Queue.Empty();
		}
	}

#endif
}

bool FFF_Capture_Thread_Window::Callback_Cursor_Draw()
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
		GetIconInfo(cursor_info.hCursor, &icon_info);

		// We have additional left and top sum because they fix multiple screen problem.
		const int CursorPos_X = (cursor_info.ptScreenPos.x - Rectangle_Current.left - Rectangle_Current.left - icon_info.xHotspot) + Rectangle_Current.left;
		const int CursorPos_Y = (cursor_info.ptScreenPos.y - Rectangle_Current.top - Rectangle_Current.top - icon_info.yHotspot) + Rectangle_Current.top;

		BITMAP bmpCursor;
		memset(&bmpCursor, 0, sizeof(bmpCursor));
		GetObjectA(icon_info.hbmColor, sizeof(CapturedBitmap), &bmpCursor);

		return DrawIconEx(DC_Destination, CursorPos_X, CursorPos_Y, cursor_info.hCursor, bmpCursor.bmWidth, bmpCursor.bmHeight, 0, NULL, DI_NORMAL);
	}

	else
	{
		return false;
	}

#else
	return false;
#endif
}

bool FFF_Capture_Thread_Window::IsWindowSizeChanged()
{
#ifdef _WIN64

	GetWindowRect(TargetWindow, &Rectangle_Current);

	FVector2D Size_Last;
	Size_Last.X = Rectangle_Last.right - Rectangle_Last.left;
	Size_Last.Y = Rectangle_Last.bottom - Rectangle_Last.top;

	FVector2D Size_Current;
	Size_Current.X = Rectangle_Current.right - Rectangle_Current.left;
	Size_Current.Y = Rectangle_Current.bottom - Rectangle_Current.top;

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