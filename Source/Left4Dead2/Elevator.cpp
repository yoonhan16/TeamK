// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "TimerManager.h"

// Sets default values
AElevator::AElevator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Elevator = CreateDefaultSubobject<USceneComponent>(TEXT("Elevator"));
	RootComponent = Elevator;

	FloorCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("FloorCollision"));
	FloorCollision->SetupAttachment(RootComponent);

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->SetupAttachment(FloorCollision);

	Front1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Front1"));
	Front1->SetupAttachment(RootComponent);

	Front2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Front2"));
	Front2->SetupAttachment(RootComponent);

	Ceiling = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ceiling"));
	Ceiling->SetupAttachment(RootComponent);

	Door1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door1"));
	Door1->SetupAttachment(RootComponent);

	Door2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door2"));
	Door2->SetupAttachment(RootComponent);

	Mirror = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mirror"));
	Mirror->SetupAttachment(RootComponent);

	Wall1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall1"));
	Wall1->SetupAttachment(RootComponent);

	Wall2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall2"));
	Wall2->SetupAttachment(RootComponent);

	InnerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InnerCollision"));
	InnerCollision->SetupAttachment(FloorCollision);

	InnerButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InnerButton"));
	InnerButton->SetupAttachment(InnerCollision);

	Light1 = CreateDefaultSubobject<URectLightComponent>(TEXT("Light1"));
	Light1->SetupAttachment(RootComponent);

	Light2 = CreateDefaultSubobject<URectLightComponent>(TEXT("Light2"));
	Light2->SetupAttachment(RootComponent);

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));

	ElevatorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ElevatorTimeline"));

}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();

	FOnTimelineFloat TimelineProgress1;
	TimelineProgress1.BindUFunction(this, FName("ControlDoor"));
	DoorTimeline->AddInterpFloat(ElevatorDoorCurveFloat, TimelineProgress1);

	FOnTimelineFloat TimelineProgress2;
	TimelineProgress2.BindUFunction(this, FName("ControlElevator"));
	ElevatorTimeline->AddInterpFloat(ElevatorCurveFloat, TimelineProgress2);

}

// Called every frame
void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Timeline1.TickTimeline(DeltaTime);
	Timeline.TickTimeline(DeltaTime);

}

void AElevator::MyInteract_Implementation()
{
	CloseDoor();

	GetWorldTimerManager().SetTimer(TimerHandle1, this, &AElevator::GoUp, 2.f, false);

	GetWorldTimerManager().SetTimer(TimerHandle2, this, &AElevator::OpenDoor, 12.f, false);

}

void AElevator::ControlElevator(float Value)
{
	FVector ElevatorInitialLocation = FVector(865.f, 2420.f, 1350.f);
	FVector ElevatorTargetLocation = FVector(865.f, 2420.f, 2305.f);

	FVector NewElevatorLocation = FMath::Lerp(ElevatorInitialLocation, ElevatorTargetLocation, Value);
	Elevator->SetRelativeLocation(NewElevatorLocation);

	UE_LOG(LogTemp, Warning, TEXT("Elevator is Moving!"));
}

void AElevator::ControlDoor(float Value)
{
	FVector LeftDoorInitialLocation = FVector(-180.f, 86.f, 200.f);
	FVector LeftDoorTargetLocation = FVector(-180.f - Location, 86.f, 200.f);
	FVector RightDoorInitialLocation = FVector(-50.f, 86.f, 200.f);
	FVector RightDoorTargetLocation = FVector(-50.f + Location, 86.f, 200.f);

	FVector NewLeftDoorLocation = FMath::Lerp(LeftDoorInitialLocation, LeftDoorTargetLocation, Value);
	FVector NewRightDoorLocation = FMath::Lerp(RightDoorInitialLocation, RightDoorTargetLocation, Value);
	Door1->SetRelativeLocation(NewLeftDoorLocation);
	Door2->SetRelativeLocation(NewRightDoorLocation);

}

void AElevator::OpenDoor()
{
	if (!DoorTimeline->IsPlaying())
	{
		DoorTimeline->PlayFromStart();
	}
}

void AElevator::CloseDoor()
{
	if (!DoorTimeline->IsPlaying())
	{
		DoorTimeline->ReverseFromEnd();
	}

}

void AElevator::GoUp()
{
	if (!ElevatorTimeline->IsPlaying())
	{
		ElevatorTimeline->PlayFromStart();
		UE_LOG(LogTemp, Warning, TEXT("Elevator is Going Up!"));
	}
}

void AElevator::LightOff()
{
	Light1->SetVisibility(false);
	Light2->SetVisibility(false);
}

void AElevator::LightOn()
{
	Light1->SetVisibility(true);
	Light2->SetVisibility(true);
}
