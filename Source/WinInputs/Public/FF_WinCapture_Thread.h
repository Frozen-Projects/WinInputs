// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// UE Includes.
#include "HAL/Runnable.h"

// Fordward Declerations.
class FRunnableThread;
class AFF_WinCapture;

struct FCapturedWindowDatas
{

public:

	uint8* Buffer = nullptr;
	size_t BufferSize = 0;
	FVector2D Resolution = FVector2D();
};

class FFF_WinCapture_Thread : public FRunnable
{
	
public:	
	// Sets default values for this actor's properties
	FFF_WinCapture_Thread(AFF_WinCapture* In_Parent_Actor);

	// Destructor.
	virtual ~FFF_WinCapture_Thread() override;

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Toggle(bool bIsPause);

protected:
	
	bool bStartThread = false;
	FRunnableThread* RunnableThread = nullptr;
	AFF_WinCapture* ParentActor = nullptr;

private:

	virtual FCapturedWindowDatas Callback_Buffer_GDI_1(FString& Error);
	virtual FCapturedWindowDatas Callback_Buffer_GDI_2(FString& Error);
	virtual FCapturedWindowDatas Callback_Buffer_DX(FString& Error);
	
};