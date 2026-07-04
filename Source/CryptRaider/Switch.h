// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mover.h"
#include "Switch.generated.h"

UCLASS()
class CRYPTRAIDER_API ASwitch : public AActor
{
	GENERATED_BODY()

	

	
public:	
	// Sets default values for this actor's properties
	ASwitch();

	void ActivateSwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SwitchBase;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SwitchHandle;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SwitchStand;


	UMover* Mover;

	FRotator SwitchInitialRotation;
	FRotator SwitchFinalRotation;

	FRotator SwitchTargetRotation;

	bool bMoveSwitch = false;
	bool bActivated = false;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	AActor* MoveActor;

};
