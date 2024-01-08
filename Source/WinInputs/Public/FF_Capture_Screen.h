// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// UE Includes
#include "HAL/RunnableThread.h"
#include "Containers/Queue.h"
#include "GenericPlatform/GenericApplication.h"		// Get monitor infos to select.

// Custom Includes
#include "WinInputsBPLibrary.h"
#include "FF_Capture_Screen_Thread.h"

#include "FF_Capture_Screen.generated.h"

UCLASS()
class WININPUTS_API AFF_Capture_Screen : public AActor
{
	GENERATED_BODY()
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#ifdef _WIN64

	class FFF_Capture_Screen_Thread* Thread_Screen_Capture = nullptr;
	virtual void GenerateTexture();

#endif

public:	

	// Sets default values for this actor's properties
	AFF_Capture_Screen();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString ThreadName;
	TQueue<FCapturedWindowDatas> Data_Queue;
	FMonitorInfo TargetMonitorInfo;
	FCapturedWindowDatas CapturedWindowDatas;

public:

	UPROPERTY(BlueprintReadWrite, meta = (ToolTip = "", ExposeOnSpawn = "true"))
	int32 MonitorIndex = 0;

	UPROPERTY(BlueprintReadWrite, meta = (ToolTip = "", ExposeOnSpawn = "true"))
	bool bShowCursor = true;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* CapturedTexture = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCaptureStarted = false;

	UPROPERTY(BlueprintAssignable)
	FDelegateDesktopCapture DelegateScreenCapture;

	UFUNCTION(BlueprintCallable)
	virtual bool Screen_Capture_Start();

	UFUNCTION(BlueprintCallable)
	virtual void Screen_Capture_Stop();

	UFUNCTION(BlueprintCallable)
	virtual bool Screen_Capture_Toggle(bool bIsPause);

};