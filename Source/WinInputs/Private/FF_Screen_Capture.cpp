// Fill out your copyright notice in the Description page of Project Settings.

#include "FF_Capture_Screen.h"

// Sets default values
AFF_Capture_Screen::AFF_Capture_Screen()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	this->PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFF_Capture_Screen::BeginPlay()
{
	Super::BeginPlay();
}

void AFF_Capture_Screen::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
#ifdef _WIN64
	this->Screen_Capture_Stop();
#endif

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AFF_Capture_Screen::Tick(float DeltaTime)
{
#ifdef _WIN64

	if (bIsCaptureStarted)
	{
		this->GenerateTexture();
	}

#endif
	Super::Tick(DeltaTime);
}

bool AFF_Capture_Screen::Screen_Capture_Start()
{
	FDisplayMetrics Display;
	FDisplayMetrics::RebuildDisplayMetrics(Display);
	MonitorIndex = FMath::Clamp(MonitorIndex, 0, Display.MonitorInfo.Num() - 1);
	TargetMonitorInfo = Display.MonitorInfo[MonitorIndex];

	this->ThreadName = "Thread_WinCap_" + FString::FromInt(FMath::RandRange(0, 9999));
	this->Thread_Screen_Capture = new FFF_Capture_Screen_Thread(this);

	if (!this->Thread_Screen_Capture)
	{
		this->ThreadName = "";
		delete this->Thread_Screen_Capture;
		return false;
	}

	this->bIsCaptureStarted = true;

	return true;
}

void AFF_Capture_Screen::Screen_Capture_Stop()
{
	if (this->Thread_Screen_Capture)
	{
		this->bIsCaptureStarted = false;
		delete this->Thread_Screen_Capture;
	}

	if (this->CapturedTexture)
	{
		this->CapturedTexture->ReleaseResource();
	}

	this->Data_Queue.Empty();
}

bool AFF_Capture_Screen::Screen_Capture_Toggle(bool bIsPause)
{
	if (Thread_Screen_Capture)
	{
		Thread_Screen_Capture->Toggle(bIsPause);
		return true;
	}

	else
	{
		return false;
	}
}

void AFF_Capture_Screen::GenerateTexture()
{
	if (!this->Data_Queue.Dequeue(CapturedWindowDatas))
	{
		UE_LOG(LogTemp, Warning, TEXT("There is a problem to dequeue captured window datas."));
		return;
	}

	if (!CapturedWindowDatas.IsDataValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Captured window buffer is invalid."));
		return;
	}

	if (!this->CapturedTexture)
	{
		this->CapturedTexture = UTexture2D::CreateTransient(CapturedWindowDatas.Resolution.X, CapturedWindowDatas.Resolution.Y, PF_B8G8R8A8);
		this->CapturedTexture->NeverStream = true;
		this->CapturedTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		this->CapturedTexture->UpdateResource();

		return;
	}

	else
	{
		const auto Region = new FUpdateTextureRegion2D(0, 0, 0, 0, CapturedWindowDatas.Resolution.X, CapturedWindowDatas.Resolution.Y);
		this->CapturedTexture->UpdateTextureRegions(0, 1, Region, 4 * CapturedWindowDatas.Resolution.X, 4, CapturedWindowDatas.Buffer);

		DelegateScreenCapture.Broadcast();
		
		return;
	}
}