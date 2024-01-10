// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// UE Includes
#include "HAL/RunnableThread.h"
#include "Containers/CircularQueue.h"
#include "GenericPlatform/GenericApplication.h"		// Get monitor infos to select.

// Custom Includes
#include "WinInputsBPLibrary.h"
#include "CP_Screen/FF_Capture_Screen_Thread.h"

#include "FF_Capture_Screen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateCaptureScreen);

UCLASS()
class WININPUTS_API AFF_Capture_Screen : public AActor
{
	GENERATED_BODY()
	
protected:

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Called when the game starts or when destroyed.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Callbacks.
	virtual bool InitCapture();
	virtual void ReleaseCapture();
	virtual void GenerateTexture();
	
	FTimerHandle Timer_Handle_Capture;
	class FFF_Capture_Thread_Screen* Capture_Thread_Screen = nullptr;

public:	

	// Sets default values for this actor's properties.
	AFF_Capture_Screen();

	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	// Transfer Queue.
	TCircularQueue<FCapturedData> Data_Queue = TCircularQueue<FCapturedData>(1024);

public:

	float Rate = 0.f;
	FString ThreadName;
	FMonitorInfo TargetMonitorInfo;
	
	UPROPERTY(BlueprintReadWrite, meta = (ToolTip = "", ExposeOnSpawn = "true"))
	int32 MonitorIndex = 0;

	UPROPERTY(BlueprintReadWrite, meta = (ToolTip = "", ExposeOnSpawn = "true"))
	bool bShowCursor = false;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* CapturedTexture = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCaptureStarted = false;

	UPROPERTY(BlueprintAssignable)
	FDelegateCaptureScreen DelegateCaptureScreen;

	UFUNCTION(BlueprintCallable)
	virtual bool Screen_Capture_Start();

	UFUNCTION(BlueprintCallable)
	virtual void Screen_Capture_Stop();

	UFUNCTION(BlueprintCallable)
	virtual bool Screen_Capture_Toggle(bool bIsPause);

};