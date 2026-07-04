// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "Lever.h"
#include "Switch.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	// if(PhysicsHandle!=nullptr){
	// 		UE_LOG(LogTemp, Display, TEXT("Got Physics Handle: %s"), *PhysicsHandle->GetName());
	// }
	// else{
	// 	UE_LOG(LogTemp, Warning, TEXT("No Physics Handle found on %s"), *GetOwner()->GetName());
	// }

	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// FRotator MyRotation = GetComponentRotation();
	// FString RotationString = MyRotation.ToCompactString();
	// UE_LOG(LogTemp, Display, TEXT("Grabber Rotation: %s"), *RotationString);

	// float Time = GetWorld()->TimeSeconds;
	// UE_LOG(LogTemp, Display, TEXT("Current Time is : %f"), Time);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	// if(PhysicsHandle==nullptr){
	// 	return;
	// }

	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent() !=nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetWorld()->GetFirstPlayerController()->GetControlRotation().Vector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetWorld()->GetFirstPlayerController()->GetControlRotation());
	}



}

void UGrabber::Grab()
{
	
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle==nullptr){
		return;
	}

	FHitResult HitResult;

	bool HasHit = GetGrabbableInReach(HitResult);
	if(HasHit){
		// AActor* HitActor = HitResult.GetActor();
		// DrawDebugSphere(
		// 	GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 5
		// );

		// DrawDebugSphere(
		// 	GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 5
		// );

		// UE_LOG(LogTemp, Display, TEXT("Hit Actor: %s"), *HitActor->GetActorNameOrLabel());
		//

		UPrimitiveComponent* HitComponent = HitResult.GetComponent();

		AActor* HitActor = HitResult.GetActor();

		ALever* Lever = Cast<ALever>(HitActor);
		ASwitch* Switch = Cast<ASwitch>(HitActor);

		if(Lever){
			Lever->ActivateLever();
			return;
		}
		if(Switch){
			Switch->ActivateSwitch();
			return;
		}

		HitComponent->SetSimulatePhysics(true);
		HitComponent -> WakeAllRigidBodies();

		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);


		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);		


	}

	// else{
	// 	UE_LOG(LogTemp, Display, TEXT("No Actor Hit"));
	// }
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	// if(PhysicsHandle == nullptr)
	// {
	// 	return;
	// }

	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		AActor* GrabbedActor =PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle-> ReleaseComponent();

	}

}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (Result == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("Grabber requires a Physics Handle component."));
	}

	return Result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start;
	FRotator Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(Start, Rotation);
	FVector End = Start + Rotation.Vector() * MaxGrabDistance;


	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	bool HasHit = GetWorld() -> SweepSingleByChannel(
		OutHitResult, 
		Start, End, 
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	);
	return HasHit;
	
}
