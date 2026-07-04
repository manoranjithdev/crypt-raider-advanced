// Fill out your copyright notice in the Description page of Project Settings.


#include "SwingObstacle.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"

// Sets default values
ASwingObstacle::ASwingObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	LeftAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("LeftAnchor"));
	LeftAnchor->SetupAttachment(RootComponent);

	RightAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("RightAnchor"));
	RightAnchor->SetupAttachment(RootComponent);

	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
	FrameMesh->SetupAttachment(RootComponent);

	SwingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwingMesh"));
	SwingMesh->SetupAttachment(RootComponent);

	LeftConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LeftConstraint"));
	LeftConstraint->SetupAttachment(RootComponent);

	RightConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RightConstraint"));
	RightConstraint->SetupAttachment(RootComponent);

	SwingMesh->SetSimulatePhysics(true);
	SwingMesh->SetNotifyRigidBodyCollision(true);

	LastSwingDirection = FVector(0.f, 1.f, 0.f);


	//HitEvent
	SwingMesh->OnComponentHit.AddDynamic(this, &ASwingObstacle::OnHit);

}

// Called when the game starts or when spawned
void ASwingObstacle::BeginPlay()
{
	Super::BeginPlay();

	SwingMesh->AddImpulse(FVector(0.0f, 150000.0f,0.0f), NAME_None, true);

	LeftConstraint->SetConstrainedComponents(
        FrameMesh,
        NAME_None,
        SwingMesh,
        NAME_None
    );

	RightConstraint->SetConstrainedComponents(
		FrameMesh,
		NAME_None,
		SwingMesh,
		NAME_None
	);

	
	
}

void ASwingObstacle::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter)
	{
		FVector PushDirection = SwingMesh->GetPhysicsLinearVelocity().GetSafeNormal();
		PushDirection.Z = 0.2f; // Add some upward force
		PushDirection.Normalize();
		PlayerCharacter->LaunchCharacter(PushDirection * 150000.0f, true, true);
	}
}

// Called every frame
void ASwingObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Velocity = SwingMesh->GetPhysicsLinearVelocity();

	float Speed = Velocity.Size();

	if (!Velocity.IsNearlyZero())
    {
        LastSwingDirection = Velocity.GetSafeNormal();
    }

	if(Speed < 250.0f && Speed > 5.0f) 
	{
		SwingMesh->AddImpulse(LastSwingDirection * 150000.0f * DeltaTime);
	}

}

