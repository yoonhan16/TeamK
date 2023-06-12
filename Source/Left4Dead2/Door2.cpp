// Fill out your copyright notice in the Description page of Project Settings.


#include "Door2.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ADoor2::ADoor2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrame->SetupAttachment(RootComponent);

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoor->SetupAttachment(DoorFrame);

	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoor->SetupAttachment(DoorFrame);
	
	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));

	SetReplicateMovement(false);
	bAlwaysRelevant = true;

}

// Called when the game starts or when spawned
void ADoor2::BeginPlay()
{
	Super::BeginPlay();
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoor2::ControlDoor);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

// Called every frame
void ADoor2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void ADoor2::MyInteract_Implementation()
{
	OpenDoor();
}

void ADoor2::OpenDoor()
{
	Server_OpenDoor();
	UE_LOG(LogTemp, Warning, TEXT("Interacted with Door!"));
}


void ADoor2::Server_OpenDoor_Implementation()
{
	Multicast_OpenDoor();
}

void ADoor2::Multicast_OpenDoor_Implementation()
{
	if (bIsDoorClosed)
	{
		//SetDoorOnSameSide();
		//Timeline.Play();
		if (!DoorTimeline->IsPlaying())
		{
			DoorTimeline->PlayFromStart();
		}
	}
	else
	{
		//Timeline.Reverse();
		if (!DoorTimeline->IsPlaying())
		{
			DoorTimeline->ReverseFromEnd();
		}
	}

	bIsDoorClosed = !bIsDoorClosed;
}

void ADoor2::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADoor2, LeftDoor);
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADoor2, RightDoor);

}

void ADoor2::ControlDoor(float Value)
{
	float Angle = bDoorOnSameSide ? -DoorRotateAngle : DoorRotateAngle;

	FRotator DoorInitialRotation = FRotator(0.f, 0.f, 0.f);
	FRotator DoorTargetRoatation = FRotator(0.f, DoorRotateAngle, 0.f);

	FRotator Rot1 = FMath::Lerp(DoorInitialRotation, DoorTargetRoatation, Value);
	FRotator Rot2 = FMath::Lerp(DoorInitialRotation, DoorTargetRoatation, Value);
	//FRotator Rot = FRotator(0.f, Angle * Value, 0.f);

	LeftDoor->SetRelativeRotation(Rot1);
	RightDoor->SetRelativeRotation(Rot2);
}
