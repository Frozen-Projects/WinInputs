// Fill out your copyright notice in the Description page of Project Settings.

#include "FF_WinCapture.h"

// Sets default values
AFF_WinCapture::AFF_WinCapture()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	this->PrimaryActorTick.bCanEverTick = true;

	this->PrimaryActorTick.TickInterval = 0.03334;
}

// Called when the game starts or when spawned
void AFF_WinCapture::BeginPlay()
{
	Super::BeginPlay();
}

void AFF_WinCapture::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
#ifdef _WIN64
	this->Window_Capture_Stop();
#endif

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AFF_WinCapture::Tick(float DeltaTime)
{
#ifdef _WIN64

	if (bIsCaptureStarted)
	{
		this->GenerateTexture();
	}

#endif
	Super::Tick(DeltaTime);
}

bool AFF_WinCapture::Window_Capture_Start()
{
	if (this->WindowName.IsEmpty())
	{
		return false;
	}

	this->ThreadName = "Thread_WinCap_" + FString::FromInt(FMath::RandRange(0, 9999));
	this->Thread_WinCapture = new FFF_WinCapture_Thread(this);

	if (!this->Thread_WinCapture)
	{
		this->ThreadName = "";
		delete this->Thread_WinCapture;
		return false;
	}

	this->bIsCaptureStarted = true;

	return true;
}

void AFF_WinCapture::Window_Capture_Stop()
{
	if (this->Thread_WinCapture)
	{
		this->bIsCaptureStarted = false;
		delete this->Thread_WinCapture;
	}

	this->CapturedTexture->ReleaseResource();
}

void AFF_WinCapture::GenerateTexture()
{
	if (!this->Data_Queue.Dequeue(CapturedWindowDatas))
	{
		UE_LOG(LogTemp, Warning, TEXT("There is a problem to dequeue captured window datas."));
		return;
	}

	if (!CapturedWindowDatas.Buffer || CapturedWindowDatas.BufferSize == 0)
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

		DelegateWindowCapture.Broadcast();
		
		return;
	}
}