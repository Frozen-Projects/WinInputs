// Fill out your copyright notice in the Description page of Project Settings.

#include "CP_Window/FF_Capture_Window.h"

// Sets default values
AFF_Capture_Window::AFF_Capture_Window()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	this->PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFF_Capture_Window::BeginPlay()
{
	Super::BeginPlay();
}

void AFF_Capture_Window::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
#ifdef _WIN64
	this->Window_Capture_Stop();
#endif

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AFF_Capture_Window::Tick(float DeltaTime)
{
#ifdef _WIN64

	if (bIsCaptureStarted)
	{
		this->GenerateTexture();
	}

#endif
	Super::Tick(DeltaTime);
}

bool AFF_Capture_Window::Window_Capture_Start()
{
#ifdef _WIN64
	if (this->WindowName.IsEmpty())
	{
		return false;
	}

	HWND TargetWindow = FindWindowExA(NULL, NULL, NULL, TCHAR_TO_UTF8(*this->WindowName));

	if (!TargetWindow)
	{
		return false;
	}
	
	DWORD TargetWindowProcessId = GetWindowThreadProcessId(TargetWindow, NULL);

	if (TargetWindowProcessId == 0)
	{
		return false;
	}

	this->ThreadName = "Thread_CP_WIN_" + FString::FromInt((int32)TargetWindowProcessId);
	this->Capture_Thread_Window = new FFF_Capture_Thread_Window(this);

	DeleteObject(TargetWindow);
	TargetWindow = nullptr;

	if (!this->Capture_Thread_Window)
	{
		this->ThreadName = "";
		delete this->Capture_Thread_Window;
		return false;
	}

	this->bIsCaptureStarted = true;

	return true;
#else
	return false;
#endif
}

void AFF_Capture_Window::Window_Capture_Stop()
{
#ifdef _WIN64
	if (this->Capture_Thread_Window)
	{
		this->bIsCaptureStarted = false;
		delete this->Capture_Thread_Window;
	}

	if (this->CapturedTexture)
	{
		this->CapturedTexture->ReleaseResource();
	}

	this->Data_Queue.Empty();
#endif
}

bool AFF_Capture_Window::Window_Capture_Toggle(bool bIsPause)
{
#ifdef _WIN64
	if (Capture_Thread_Window)
	{
		Capture_Thread_Window->Toggle(bIsPause);
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

void AFF_Capture_Window::GenerateTexture()
{
#ifdef _WIN64
	FCapturedData EachData;
	if (!this->Data_Queue.Dequeue(EachData))
	{
		UE_LOG(LogTemp, Warning, TEXT("There is a problem to dequeue captured window data."));
		return;
	}

	if (!EachData.IsDataValid())
	{
		// We don't need old data if dequeued buffer is invalid.
		this->Data_Queue.Empty();

		UE_LOG(LogTemp, Warning, TEXT("Captured window buffer is invalid."));
		return;
	}

	if (!this->CapturedTexture || LastResolution != EachData.Resolution)
	{
		this->CapturedTexture = UTexture2D::CreateTransient(EachData.Resolution.X, EachData.Resolution.Y, PF_B8G8R8A8);
		this->CapturedTexture->NeverStream = true;
		this->CapturedTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		this->CapturedTexture->UpdateResource();

		LastResolution = FVector2D(EachData.Resolution.X, EachData.Resolution.Y);

		// We don't need old data after re-creating texture.
		this->Data_Queue.Empty();

		DelegateCaptureWindow.Broadcast(EachData.WindowLocation);

		return;
	}

	else
	{
		const auto Region = new FUpdateTextureRegion2D(0, 0, 0, 0, EachData.Resolution.X, EachData.Resolution.Y);
		this->CapturedTexture->UpdateTextureRegions(0, 1, Region, 4 * EachData.Resolution.X, 4, EachData.Buffer);

		// We don't need old data after updating texture.
		this->Data_Queue.Empty();

		DelegateCaptureWindow.Broadcast(EachData.WindowLocation);
		
		return;
	}
#endif
}