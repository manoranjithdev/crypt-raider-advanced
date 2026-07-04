// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lever.generated.h"

class UStaticMeshComponent;
class ARotatingBridge;

UCLASS()
class CRYPTRAIDER_API ALever : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	ALever();

	void ActivateLever();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LeverBase;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LeverHandle;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LeverStand;

	UPROPERTY(EditAnywhere)
	ARotatingBridge* RotatingBridge;

	bool bCanActivate = true;

	bool bLeverActivated = false;

	FRotator TargetRotation;

	bool bRotateLever = false;

	FRotator LeverRotation = FRotator(0.0f, 90.0f, 0.0f);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
