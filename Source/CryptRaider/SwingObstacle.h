// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwingObstacle.generated.h"


class UStaticMeshComponent;
class UPhysicsConstraintComponent;

UCLASS()
class CRYPTRAIDER_API ASwingObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwingObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* FrameMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* SwingMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhysicsConstraintComponent* LeftConstraint;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhysicsConstraintComponent* RightConstraint;

	UPROPERTY(VisibleAnywhere, Category = "Components")	USceneComponent* LeftAnchor;

	UPROPERTY(VisibleAnywhere, Category = "Components")	USceneComponent* RightAnchor;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector LastSwingDirection;

};
