// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Mover.h"
#include "Components/BoxComponent.h"
#include "RotatingBridge.h"

UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

    PressurePlate = Cast<UStaticMeshComponent>(GetOwner()->GetDefaultSubobjectByName(PlateMeshName));

    if(PressurePlate)
    {
        DynamicMaterial = PressurePlate->CreateAndSetMaterialInstanceDynamic(0);

        CurrentGlow = 0.f;
        TargetGlow = 0.f;

        if(DynamicMaterial)
        {
        DynamicMaterial->SetScalarParameterValue("GlowStrength",0.f);
        }

        PlateInitialLocation = PressurePlate->GetRelativeLocation();
        PlatePressedLocation = PlateInitialLocation - FVector(0.f, 0.f, 17.f); 
    }    

    TriggerBox = GetOwner()->FindComponentByClass<UBoxComponent>();

    if(!TriggerBox)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing PressurePlate or TriggerBox"));
		return;
	}

    


    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnComponentBeginOverlap);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OnComponentEndOverlap);

    if(TargetActor)
    {
        Mover = TargetActor->FindComponentByClass<UMover>();
    }
    else
    {
        Mover = GetOwner()->FindComponentByClass<UMover>();
    }
    

    if (Mover == nullptr && RotatingBridge == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("TriggerComponent requires a Mover or RotatingBridge component."));
    }


}


	void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
    {
        Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

        AActor* Actor = GetAcceptableActor();
        if(Actor != nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("Actor Overlapping"));

            UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
            if(Component)
            {
                Component->SetSimulatePhysics(false);
            }

          
            Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

              if(RotatingBridge && !bHasTriggered)
            {
                RotatingBridge->ToggleBridge();
                bHasTriggered = true;
            }


            if (Mover)  
            {
             Mover->SetShouldMove(true);
             UE_LOG(LogTemp, Warning, TEXT("Mover Found and Activated"));
            }
            }
            else
            {
            bHasTriggered = false;
            
            if (Mover)
            {
                Mover->SetShouldMove(false);
            }

            
    //     AActor* Actor = GetAcceptableActor();

    // if (Mover)
    // {
    //     Mover->SetShouldMove(Actor != nullptr);
    // }
            }

// ------------- Pressure Plate --------------
        if(PressurePlate){
            
            FVector CurrentLocation = PressurePlate->GetRelativeLocation();

            FVector Target = bIsPressed ? PlatePressedLocation : PlateInitialLocation;

            FVector NewLocation = FMath::VInterpTo(CurrentLocation, Target, DeltaTime, 5.f);

            PressurePlate->SetRelativeLocation(NewLocation);

            CurrentGlow = FMath::FInterpTo(CurrentGlow, TargetGlow, DeltaTime, 5.f);

            if(DynamicMaterial)
            {
                DynamicMaterial->SetScalarParameterValue("GlowStrength", CurrentGlow);
            }
        }

    }
        void UTriggerComponent::SetMover(UMover* NewMover)
        {
            Mover = NewMover;
        }
        
void UTriggerComponent::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult& SweepResult)
        {
            bIsPressed = true;
            TargetGlow = 0.0005f;
        }
        
void UTriggerComponent::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex)
        {
            bIsPressed = false;
            TargetGlow = 0.f;
        }


        // int32 index;


        // for(index =0; index<Actors.Num(); ++index)
        // {
        //     FString ActorName = Actors[index]->GetActorNameOrLabel();
        //     UE_LOG(LogTemp, Display, TEXT("Overlapping: %s"), *ActorName);
        // }

        //UNREAL SPECIFIC FOR LOOP
        // for(AActor* Actor : Actors)
        // {
        //      if(Actor->ActorHasTag(AcceptableActorTag))
        //      {
        //         UE_LOG(LogTemp, Display, TEXT("Unlocking1"));
        //      }
        //     //  UE_LOG(LogTemp, Display, TEXT("Overlapping: %s"), *ActorName);
        // }
        

AActor* UTriggerComponent::GetAcceptableActor() const
{

    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    UE_LOG(LogTemp, Warning, TEXT("Overlap Count: %d"), Actors.Num());
    for(AActor* Actor : Actors)
    {
        bool bHasAcceptableTag = false;
        for(const FName& Tag : AcceptableActorTag)
        {
            if(Actor->ActorHasTag(Tag))
            {
                bHasAcceptableTag = true;
                break;
            }
        }
        
        bool IsGrabbed = Actor->ActorHasTag("Grabbed");

        if(bHasAcceptableTag && !IsGrabbed)
        {
            return Actor;
        }
    }
   return nullptr;


    // TArray<AActor*> Actors;
    // GetOverlappingActors(Actors);

    // for (AActor* Actor : Actors)
    // {
    //     if (Actor->ActorHasTag(AcceptableActorTag))
    //     {
    //         return Actor;
    //     }
    // }

    // return nullptr;

    // TArray<AActor*> Actors;
    // GetOverlappingActors(Actors);

    // UE_LOG(LogTemp, Warning, TEXT("Overlap Count: %d"), Actors.Num());

    // if (Actors.Num() > 0)
    // {
    //     return Actors[0];
    // }

    // return nullptr;

}
