// Fill out your copyright notice in the Description page of Project Settings.

#include "Generator.h"


// Sets default values
AGenerator::AGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	Generator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Generator"));
	Generator->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();

	// OutArray 배열에 레벨 위에 있는 모든 전등 액터들 저장
	TArray<AActor*> OutArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACeilingLight::StaticClass(), OutArray);
	for (AActor* Actor : OutArray)
	{
		LightArray.Add(Cast<ACeilingLight>(Actor));
	}
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGenerator::MyInteract_Implementation()
{
	if (HasAuthority())
	{
		GeneratorSound();

		// 발전기에 상호작용 시 2분 후에 탈출에 필요한 함수들이 실행되도록 타이머 설정
		FTimerManager& TimerManager = GetWorldTimerManager();
		
		TimerManager.SetTimer(TimerHandle, this, &AGenerator::GeneratorOn, 120.f, false);
		
		TimerManager.SetTimer(TimerHandle, this, &AGenerator::CollapseFloorDelayed, 120.f, false);
		UE_LOG(LogTemp, Warning, TEXT("The floor(1st) is Collapsed!"));

		TimerManager.SetTimer(TimerHandle, this, &AGenerator::SetTaskCheckDelayed, 120.f, false);
		UE_LOG(LogTemp, Warning, TEXT("TaskCheck"));

		TimerManager.SetTimer(TimerHandle, this, &AGenerator::LightOnDelayed, 120.f, false);
	}
}

// 전등을 켜는 함수
void AGenerator::GeneratorOn()
{
	if (LightArray.Num() > 0)
	{
		for (ACeilingLight* Light : LightArray)
		{
			Light->LightOn();
		}
	}
}

void AGenerator::CollapseFloorDelayed()
{
	if (CollapsingFloor)
	{
		CollapsingFloor->FloorCollapsing();
		UE_LOG(LogTemp, Warning, TEXT("The floor(1st) is Collapsed!"));
	}
}

void AGenerator::SetTaskCheckDelayed()
{
	if (TaskCheck)
	{
		TaskCheck->SetTask(true);
		UE_LOG(LogTemp, Warning, TEXT("TaskCheck"));
	}
}

void AGenerator::LightOnDelayed()
{
	if (Elevator)
	{
		Elevator->LightOn();
	}
}

void AGenerator::GeneratorSound()
{
	Server_GeneratorSound();
}

void AGenerator::Server_GeneratorSound_Implementation()
{
	NetMulticast_GeneratorSound();
}

bool AGenerator::Server_GeneratorSound_Validate()
{
	return true;
}

void AGenerator::NetMulticast_GeneratorSound_Implementation()
{
	GeneratorSoundAttenuation(5000.f);

	UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, Sound, GetActorLocation());
}

// 액터로부터 거리가 멀어질수록 소리 감쇠
void AGenerator::GeneratorSoundAttenuation(float MaxDistance)
{
	if (Sound)
	{
		USoundAttenuation* AttenuationSettings = NewObject<USoundAttenuation>(this, USoundAttenuation::StaticClass());
		AttenuationSettings->Attenuation.FalloffDistance = MaxDistance;
		Sound->AttenuationSettings = AttenuationSettings;
	}
}

void AGenerator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGenerator, Sound);
}
