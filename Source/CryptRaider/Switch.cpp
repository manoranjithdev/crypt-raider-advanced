// Fill out your copyright notice in the Description page of Project Settings.


#include "Switch.h"
#include "Mover.h"

// Sets default values
ASwitch::ASwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SwitchBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchBase"));
	SwitchBase->SetupAttachment(RootComponent);

	SwitchStand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchStand"));
	SwitchStand -> SetupAttachment(SwitchBase);


	SwitchHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchHandle"));
	SwitchHandle->SetupAttachment(SwitchBase);



}

// Called when the game starts or when spawned
void ASwitch::BeginPlay()
{
	Super::BeginPlay();

	if(MoveActor)
	{
		Mover = MoveActor->FindComponentByClass<UMover>();
	}

	SwitchInitialRotation = SwitchHandle->GetRelativeRotation();

	SwitchFinalRotation = SwitchInitialRotation + FRotator(90.f,0.f,0.f);

	SwitchTargetRotation = SwitchInitialRotation;
	
}

// Called every frame
void ASwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bMoveSwitch)
	{
		FRotator CurrentRotation = SwitchHandle->GetRelativeRotation();
		
		FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation,SwitchTargetRotation,DeltaTime,120.f);

		SwitchHandle->SetRelativeRotation(NewRotation);

		if(NewRotation.Equals(SwitchTargetRotation,1.f))
		{
			SwitchHandle -> SetRelativeRotation(SwitchTargetRotation);

			bMoveSwitch = false;
		}
	}

}


void ASwitch::ActivateSwitch()
{
	bActivated = !bActivated;

	SwitchTargetRotation = bActivated ? SwitchFinalRotation : SwitchInitialRotation;

	bMoveSwitch = true;

	if(Mover)
	{
		Mover->ToggleMove();
	}
}
