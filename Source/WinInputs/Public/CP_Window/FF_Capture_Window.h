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
#include "CP_Window/FF_Capture_Window_Thread.h"

#include "FF_Capture_Window.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateCaptureWindow, FVector2D, TargetPosition);

UCLASS()
class WININPUTS_API AFF_Capture_Window : public AActor
{
	GENERATED_BODY()
	
protected:

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Called when the game finished or when destroyed.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Callbacks.

	virtual bool InitCapture();
	virtual void ReleaseCapture();
	virtual void GenerateTexture();

	FVector2D LastResolution;
	FTimerHandle Timer_Handle_Capture;
	class FFF_Capture_Thread_Window* Capture_Thread_Window = nullptr;
	
	//uint8* BGRA_Buffer = nullptr;

public:

	// Sets default values for this actor's properties.
	AFF_Capture_Window();

	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	// Transfer Queue.
	TCircularQueue<FCapturedDataWindow> Data_Queue = TCircularQueue<FCapturedDataWindow>(1024);

public:	

	float Rate = 0.f;
	FString ThreadName;
	
	UPROPERTY(BlueprintReadWrite, meta = (ToolTip = "", ExposeOnSpawn = "true"))
	FString WindowName;

	UPROPERTY(BlueprintReadWrite, meta = (ToolTip = "", ExposeOnSpawn = "true"))
	bool bShowCursor = false;

	UPROPERTY(BlueprintReadWrite, meta = (ToolTip = "Enable Hardware Accelerated Window Compability", ExposeOnSpawn = "true"))
	bool bUseHaCompability = false;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* CapturedTexture = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCaptureStarted = false;

	UPROPERTY(BlueprintAssignable)
	FDelegateCaptureWindow DelegateCaptureWindow;

	UFUNCTION(BlueprintCallable)
	virtual bool Window_Capture_Start();

	UFUNCTION(BlueprintCallable)
	virtual void Window_Capture_Stop();

	UFUNCTION(BlueprintCallable)
	virtual bool Window_Capture_Toggle(bool bIsPause);

};