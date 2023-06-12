// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;
	BoxCollision->SetBoxExtent(FVector(10.0f, 53.0f, 110.0f));

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrame->SetupAttachment(RootComponent);

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(DoorFrame);
	Door->SetIsReplicated(true);
	Door->SetNetAddressable();
	Door->SetShouldUpdatePhysicsVolume(true);
	Door->SetNetAddressable();
	Door->SetOwnerNoSee(false);

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));

	SetReplicateMovement(true);
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("ControlDoor"));
		DoorTimeline->AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoor::MyInteract_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Interacted with Door!"));

	OpenDoor();

}

void ADoor::OpenDoor()
{
	Server_OpenDoor();
}

void ADoor::Server_OpenDoor_Implementation()
{
	Multicast_OpenDoor();
}

bool ADoor::Server_OpenDoor_Validate()
{
	return true;
}

void ADoor::Multicast_OpenDoor_Implementation()
{
	if (bIsDoorClosed)
	{
		if (!DoorTimeline->IsPlaying())
		{
			DoorTimeline->PlayFromStart();
		}
	}
	else
	{
		if (!DoorTimeline->IsPlaying())
		{
			DoorTimeline->ReverseFromEnd();
		}
	}

	bIsDoorClosed = !bIsDoorClosed;
	UE_LOG(LogTemp, Warning, TEXT("Interacted with Door!"));
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ADoor, Door, COND_SimulatedOnly);
	DOREPLIFETIME(ADoor, bIsDoorClosed);

}

void ADoor::OnRep_IsDoorClosed()
{
	if (bIsDoorClosed)
	{
		DoorTimeline->PlayFromStart();
	}
	else
	{
		DoorTimeline->ReverseFromEnd();
	}
}

void ADoor::ControlDoor(float Value)
{
	FRotator DoorInitialRotation = FRotator(0.f, 0.f, 0.f);
	FRotator DoorTargetRoatation = FRotator(0.f, DoorRotateAngle, 0.f);

	FRotator Rot = FMath::Lerp(DoorInitialRotation, DoorTargetRoatation, Value);
	//FRotator Rot = FRotator(0.f, Angle * Value, 0.f);

	Door->SetRelativeRotation(Rot);
}

