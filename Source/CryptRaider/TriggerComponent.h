// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TriggerComponent.generated.h"

class UMover;
class ARotatingBridge;

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UTriggerComponent();

protected:
// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* Mover);

	UPROPERTY(EditAnywhere)
	ARotatingBridge* RotatingBridge;

	UFUNCTION()
	void OnComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnComponentEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
	);

	



private:

	UPROPERTY(EditAnywhere)
	TArray<FName> AcceptableActorTag;	

	UStaticMeshComponent* PlateMesh;

	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess="true"))
	FName PlateMeshName;

	UPROPERTY()
	UMover* Mover;

	UPROPERTY(EditAnywhere)
	AActor* TargetActor;

	UStaticMeshComponent* PressurePlate = nullptr;

	UBoxComponent* TriggerBox = nullptr;

	FVector PlateInitialLocation;
	FVector PlatePressedLocation;

	UMaterialInstanceDynamic* DynamicMaterial;

	bool bIsPressed = false;

	AActor* GetAcceptableActor() const;

	bool bHasTriggered = false;

	float CurrentGlow = 0.f;

	float TargetGlow = 0.f;
};
