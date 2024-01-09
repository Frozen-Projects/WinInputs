// Fill out your copyright notice in the Description page of Project Settings.

#include "CP_Screen/FF_Capture_Screen.h"

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
#ifdef _WIN64
	FDisplayMetrics Display;
	FDisplayMetrics::RebuildDisplayMetrics(Display);
	MonitorIndex = FMath::Clamp(MonitorIndex, 0, Display.MonitorInfo.Num() - 1);
	TargetMonitorInfo = Display.MonitorInfo[MonitorIndex];

	this->ThreadName = "Thread_CP_SCR_" + FString::FromInt(FMath::RandRange(0, 9999));
	this->Capture_Thread_Screen = new FFF_Capture_Thread_Screen(this);

	if (!this->Capture_Thread_Screen)
	{
		this->ThreadName = "";
		delete this->Capture_Thread_Screen;
		return false;
	}

	this->bIsCaptureStarted = true;

	return true;
#else
	return false;
#endif
}

void AFF_Capture_Screen::Screen_Capture_Stop()
{
#ifdef _WIN64
	if (this->Capture_Thread_Screen)
	{
		this->bIsCaptureStarted = false;
		delete this->Capture_Thread_Screen;
	}

	if (this->CapturedTexture)
	{
		this->CapturedTexture->ReleaseResource();
	}

	this->Data_Queue.Empty();
#endif
}

bool AFF_Capture_Screen::Screen_Capture_Toggle(bool bIsPause)
{
#ifdef _WIN64
	if (Capture_Thread_Screen)
	{
		Capture_Thread_Screen->Toggle(bIsPause);
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

void AFF_Capture_Screen::GenerateTexture()
{
#ifdef _WIN64
	FCapturedData EachData;
	if (!this->Data_Queue.Dequeue(EachData))
	{
		UE_LOG(LogTemp, Warning, TEXT("There is a problem to dequeue captured window datas."));
		return;
	}

	if (!EachData.IsDataValid())
	{
		// We don't need old datas.
		this->Data_Queue.Empty();

		UE_LOG(LogTemp, Warning, TEXT("Captured window buffer is invalid."));
		return;
	}

	if (!this->CapturedTexture)
	{
		this->CapturedTexture = UTexture2D::CreateTransient(EachData.Resolution.X, EachData.Resolution.Y, PF_B8G8R8A8);
		this->CapturedTexture->NeverStream = true;
		this->CapturedTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		this->CapturedTexture->UpdateResource();

		return;
	}

	else
	{
		const auto Region = new FUpdateTextureRegion2D(0, 0, 0, 0, EachData.Resolution.X, EachData.Resolution.Y);
		this->CapturedTexture->UpdateTextureRegions(0, 1, Region, 4 * EachData.Resolution.X, 4, EachData.Buffer);

		this->Data_Queue.Empty();

		DelegateCaptureScreen.Broadcast();
		
		return;
	}
#endif
}