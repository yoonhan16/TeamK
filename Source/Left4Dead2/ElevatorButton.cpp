// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorButton.h"
#include "Components/BoxComponent.h"

// Sets default values
AElevatorButton::AElevatorButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	Button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button"));
	Button->SetupAttachment(RootComponent);

	bReplicates = true;
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void AElevatorButton::BeginPlay()
{
	Super::BeginPlay();

	// OutArray 배열에 레벨 위에 있는 모든 전등 액터들 저장
	TArray<AActor*> OutArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACeilingLight::StaticClass(), OutArray);
	for (AActor* Actor : OutArray)
	{
		LightArray.Add(Cast<ACeilingLight>(Actor));
	}

	bDidTask = false;
}

// Called every frame
void AElevatorButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AElevatorButton::MyInteract_Implementation()
{
	// bDidTask를 검사하여 거짓일 경우 LightArray에 저장된 전등 액터들을 끄고 바닥을 사라지게 함
	// 참일 경우 엘리베이터 문 정상 작동
	if (!bDidTask)
	{
		if (LightArray.Num() > 0)
		{
			for (ACeilingLight* Light : LightArray)
			{
				Light->LightOff();
			}
		}

		if (CollapsingFloor)
		{
			CollapsingFloor->FloorCollapsing();
			UE_LOG(LogTemp, Warning, TEXT("The Room4(1st)'s floor is Collapsed!"));
			UE_LOG(LogTemp, Warning, TEXT("You should solve the Task!"));
		}

		if (Elevator)
		{
			Elevator->LightOff();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Now you're good to go!"));

		if (Elevator)
		{
			Elevator->DoorTimeline->PlayFromStart();
		}
	}
}

void AElevatorButton::SetTask(bool bFlag)
{
	bDidTask = bFlag;

}

// 리플리케이션 프로퍼티 정의
void AElevatorButton::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AElevatorButton, bDidTask);
}

