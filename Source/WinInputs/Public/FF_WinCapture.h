// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// UE Includes
#include "HAL/RunnableThread.h"
#include "Containers/Queue.h"

// Threads
#include "FF_WinCapture_Thread.h"

#include "FF_WinCapture.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateWindowCapture);

UCLASS()
class WININPUTS_API AFF_WinCapture : public AActor
{
	GENERATED_BODY()
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#ifdef _WIN64

	class FFF_WinCapture_Thread* Thread_WinCapture = nullptr;
	virtual void GenerateTexture();

#endif

public:	

	// Sets default values for this actor's properties
	AFF_WinCapture();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	FString ThreadName;
	
	TQueue<FCapturedWindowDatas> Data_Queue;
	
	FCapturedWindowDatas CapturedWindowDatas;

public:

	UPROPERTY(BlueprintReadWrite, meta = (ToolTip = "", ExposeOnSpawn = "true"))
	FString WindowName;

	UPROPERTY(BlueprintReadWrite, meta = (ToolTip = "", ExposeOnSpawn = "true"))
	int32 MonitorIndex = 0;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* CapturedTexture = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCaptureStarted = false;

	UPROPERTY(BlueprintAssignable)
	FDelegateWindowCapture DelegateWindowCapture;

	UFUNCTION(BlueprintCallable)
	virtual bool Window_Capture_Start();

	UFUNCTION(BlueprintCallable)
	virtual void Window_Capture_Stop();

};