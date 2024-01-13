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
	
	if (!this->Init_DC(Error))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Error));
		return false;
	}

	if (!this->Init_Bitmap(Error, false))
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
		this->GetCurrentRectangles();

		if (this->IsResolutionChanged())
		{
			FString Error;
			if (!this->Init_Bitmap(Error, true))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Error));
			}

			// If target window size changed, we need to clear data queue before continue.
			this->ParentActor->Data_Queue.Empty();
		}
		
		this->GDI_Buffer();
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
	this->GDI_Release();
#endif
}

void FFF_Capture_Thread_Window::Toggle(bool bIsPause)
{
	if (this->RunnableThread)
	{
		this->RunnableThread->Suspend(bIsPause);
	}
}

/*
* Callback Functions
*/

bool FFF_Capture_Thread_Window::Init_DC(FString& Error)
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
	this->bUseHaCompability = this->ParentActor->bUseHaCompability;

	return true;
#else
	return false;
#endif
}

bool FFF_Capture_Thread_Window::Init_Bitmap(FString& Error, bool bReInit)
{
#ifdef _WIN64

	if (bReInit && CapturedBitmap)
	{
		DeleteObject(CapturedBitmap);
	}
	
	RECT ClientRect_Last;
	GetClientRect(TargetWindow, &ClientRect_Last);
	DwmGetWindowAttribute(TargetWindow, DWMWA_EXTENDED_FRAME_BOUNDS, &WindowRect_Last, sizeof(WindowRect_Last));

	if (!this->bUseHaCompability)
	{
		CapturedData.WindowOffset.X = (WindowRect_Last.right - ClientRect_Last.right) - (WindowRect_Last.left - ClientRect_Last.left);
		CapturedData.WindowOffset.Y = (WindowRect_Last.bottom - ClientRect_Last.bottom) - (WindowRect_Last.top - ClientRect_Last.top);
	}

	CapturedData.Resolution.X = WindowRect_Last.right - WindowRect_Last.left;
	CapturedData.Resolution.Y = WindowRect_Last.bottom - WindowRect_Last.top;

	CapturedData.BufferSize = CapturedData.Resolution.X * CapturedData.Resolution.Y * 4;
	CapturedData.Stride = (((32 * (size_t)CapturedData.Resolution.X + 31) & ~31) / 8.f) * CapturedData.Resolution.Y;

	CapturedData.Buffer = (uint8*)malloc(CapturedData.BufferSize);

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

void FFF_Capture_Thread_Window::GetCurrentRectangles()
{
#ifdef _WIN64

	DwmGetWindowAttribute(TargetWindow, DWMWA_EXTENDED_FRAME_BOUNDS, &WindowRect_Current, sizeof(WindowRect_Current));

#endif
}

void FFF_Capture_Thread_Window::GDI_Buffer()
{
#ifdef _WIN64

	CapturedData.WindowLocation.X = WindowRect_Current.left;
	CapturedData.WindowLocation.Y = WindowRect_Current.top;

	if (this->bUseHaCompability)
	{
		PrintWindow(TargetWindow, DC_Source, 2);
	}

	if (!BitBlt(DC_Destination, 0, 0, CapturedData.Resolution.X, CapturedData.Resolution.Y, DC_Source, -CapturedData.WindowOffset.X, -CapturedData.WindowOffset.Y, SRCCOPY | CAPTUREBLT))
	{
		return;
	}

	if (bShowCursor)
	{
		if (!this->Cursor_Draw())
		{
			return;
		}
	}

	if (!TempBuffer)
	{
		return;
	}

	if (this->bUseHaCompability)
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

bool FFF_Capture_Thread_Window::Cursor_Draw()
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
		const int CursorPos_X = (cursor_info.ptScreenPos.x - WindowRect_Current.left - WindowRect_Current.left - icon_info.xHotspot) + WindowRect_Current.left;
		const int CursorPos_Y = (cursor_info.ptScreenPos.y - WindowRect_Current.top - WindowRect_Current.top - icon_info.yHotspot) + WindowRect_Current.top;

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

bool FFF_Capture_Thread_Window::IsResolutionChanged()
{
	FVector2D CurrentResolution = FVector2D(this->WindowRect_Current.right - this->WindowRect_Current.left, this->WindowRect_Current.bottom - this->WindowRect_Current.top);

	if (CurrentResolution == this->CapturedData.Resolution)
	{
		return false;
	}

	else
	{
		return true;
	}
}

void FFF_Capture_Thread_Window::GDI_Release()
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