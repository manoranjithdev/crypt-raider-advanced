// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingBridge.h"
#include "Components/StaticMeshComponent.h"
#include "Mover.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ARotatingBridge::ARotatingBridge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	// RootComponent->SetMobility(EComponentMobility::Movable);

	bCanTrigger = true;
	bIsRotating = false;
	
}

// Called when the game starts or when spawned
void ARotatingBridge::BeginPlay()
{
	Super::BeginPlay();

	TargetRotation = GetActorRotation();
	
}

// Called every frame
void ARotatingBridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// FRotator CurrentRotation = GetActorRotation();

	// FRotator TargetRotation = bShouldRotate ? OpenRotation : ClosedRotation;

	// FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 1.0f);

	// SetActorRotation(NewRotation);

    FRotator CurrentRotation = GetActorRotation();

    FRotator NewRotation = FMath::RInterpConstantTo(
        CurrentRotation,
        TargetRotation,
        DeltaTime,
        20.0f
    );

    SetActorRotation(NewRotation);

	if(GetActorRotation().Equals(TargetRotation, 1.0f))
	{	
		SetActorRotation(TargetRotation);

		bIsRotating = false;

		
	}
    
}

void ARotatingBridge::ToggleBridge()
{
	if(!bCanTrigger) return;

	bCanTrigger = false;

	UE_LOG(LogTemp, Warning, TEXT("Toggle Bridge Called"));

	TargetRotation += FRotator(0.0f, 90.0f, 0.0f);

	GetWorldTimerManager().SetTimer(
		CooldownTimer,
		this,
		&ARotatingBridge::EnableBridgeTrigger,
		6.0f,
		false
	);
}

void ARotatingBridge::EnableBridgeTrigger()
{
	 bCanTrigger = true;
	
}

