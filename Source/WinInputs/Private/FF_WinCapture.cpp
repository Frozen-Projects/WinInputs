// Fill out your copyright notice in the Description page of Project Settings.

#include "FF_WinCapture.h"

// Sets default values
AFF_WinCapture::AFF_WinCapture() : Circ_Captured_Window(1000)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	this->PrimaryActorTick.bCanEverTick = true;

	this->PrimaryActorTick.TickInterval = 0.016;
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

	this->ThreadName = "Thread_" + this->WindowName.Left(7);
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
}

void AFF_WinCapture::GenerateTexture()
{
	FCapturedWindowDatas CapturedWindowDatas;
	if (!this->Circ_Captured_Window.Dequeue(CapturedWindowDatas))
	{
		UE_LOG(LogTemp, Warning, TEXT("There is a problem to dequeue captured window datas."));
		return;
	}

	if (!CapturedWindowDatas.Buffer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Captured window buffer is invalid."));
		return;
	}

	if (!this->CapturedTexture)
	{
		this->CapturedTexture = UTexture2D::CreateTransient(CapturedWindowDatas.WindowSize.X, CapturedWindowDatas.WindowSize.Y, PF_B8G8R8A8);
		this->CapturedTexture->NeverStream = true;
		this->CapturedTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;

		FTexture2DMipMap& Texture_Mip = this->CapturedTexture->GetPlatformData()->Mips[0];
		void* Texture_Data = Texture_Mip.BulkData.Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(Texture_Data, CapturedWindowDatas.Buffer, CapturedWindowDatas.BufferSize);
		Texture_Mip.BulkData.Unlock();
		this->CapturedTexture->UpdateResource();

		DelegateWindowCapture.Broadcast();

		return;
	}

	else
	{
		ENQUEUE_RENDER_COMMAND(CaptureWindowCommand)([this, CapturedWindowDatas](FRHICommandListImmediate& CommandList)
			{
				uint32 DestStride = 0;
				FRHITexture* TextureRHI = this->CapturedTexture->GetResource()->GetTextureRHI();
				uint32_t* Buffer = (uint32_t*)RHILockTexture2D(TextureRHI, 0, EResourceLockMode::RLM_WriteOnly, DestStride, false, true);
				FMemory::Memcpy(Buffer, CapturedWindowDatas.Buffer, CapturedWindowDatas.BufferSize);
				RHIUnlockTexture2D(TextureRHI, 0, false, true);
			}
		);

		DelegateWindowCapture.Broadcast();

		return;
	}
}