// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "RotatingBridge.generated.h"

class UMover;
class UStaticMeshComponent;

UCLASS()
class CRYPTRAIDER_API ARotatingBridge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatingBridge();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleBridge();

	UPROPERTY(EditAnywhere, Category = "Bridge")
	FRotator TargetRotation;

	void EnableBridgeTrigger();

private:

	UPROPERTY(EditAnywhere, Category = "Bridge")
	UMover* MoverComponent;

	bool bShouldRotate;

	bool bCanTrigger = true;

	bool bIsRotating = false;

	float RotationSpeed = 2.0f;

	FTimerHandle CooldownTimer;



};
