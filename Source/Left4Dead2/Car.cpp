// Fill out your copyright notice in the Description page of Project Settings.


#include "Car.h"
#include "Components/BoxComponent.h"

// Sets default valu
ACar::ACar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Car = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Car"));
    RootComponent = Car;

    // �ʱ�ȭ���� ���� ��� ���� �ʱ�ȭ
    Sound = nullptr;

    bReplicates = true;
    bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void ACar::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACar::CarAlarm()
{
    Server_CarAlarm();
}

void ACar::Server_CarAlarm_Implementation()
{
    NetMulticast_CarAlarm();
}

bool ACar::Server_CarAlarm_Validate()
{
    return true;
}

void ACar::NetMulticast_CarAlarm_Implementation()
{
    if (!bIsHornSoundPlaying)
    {
        SetAlarmSoundAttenuation(2000.f);

        UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, Sound, GetActorLocation());

        // �Ҹ��� �� ���� �����ϵ��� bool�� ����
        bIsHornSoundPlaying = true;
    }
}

// ���ͷκ��� �Ÿ��� �־������� �Ҹ� ����
void ACar::SetAlarmSoundAttenuation(float MaxDistance)
{
    if (Sound)
    {
        USoundAttenuation* AttenuationSettings = NewObject<USoundAttenuation>(this, USoundAttenuation::StaticClass());
        AttenuationSettings->Attenuation.FalloffDistance = MaxDistance;
        Sound->AttenuationSettings = AttenuationSettings;
    }
}

void ACar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACar, Sound);
}
