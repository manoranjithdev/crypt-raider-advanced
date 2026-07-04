// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"
#include "RotatingBridge.h"

// Sets default values
ALever::ALever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	LeverBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverBase"));
	LeverBase->SetupAttachment(RootComponent);

	LeverStand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverStand"));
	LeverStand->SetupAttachment(LeverBase);

	LeverHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverHandle"));
	LeverHandle->SetupAttachment(LeverBase);


}

void ALever::ActivateLever()
{
	if(!bCanActivate) return;
	
	TargetRotation+=FRotator(0.0f, 90.0f, 0.0f);

	bRotateLever = true;

	if(RotatingBridge)
	{
		RotatingBridge->ToggleBridge();
	}

}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{
	Super::BeginPlay();

	TargetRotation = LeverHandle->GetRelativeRotation();
	
}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bRotateLever)
	{
		FRotator CurrentRotation = LeverHandle->GetRelativeRotation();

		FRotator NewRotation = FMath::RInterpConstantTo(
			CurrentRotation,
			TargetRotation,
			DeltaTime,
			120.f
		);

		LeverHandle->SetRelativeRotation(NewRotation);

		if(NewRotation.Equals(TargetRotation, 1.f))
		{
			LeverHandle->SetRelativeRotation(TargetRotation);

			bRotateLever = false;
		}
	}
}

