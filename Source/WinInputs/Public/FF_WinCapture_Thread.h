// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// UE Includes.
#include "HAL/Runnable.h"

// Fordward Declerations.
class FRunnableThread;
class AFF_WinCapture;

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

	virtual bool Callback_Get_Window_Buffer(FString& Error);

protected:
	
	bool bStartThread = false;
	FRunnableThread* RunnableThread = nullptr;
	
	AFF_WinCapture* ParentActor = nullptr;
	FString WindowName;

};
