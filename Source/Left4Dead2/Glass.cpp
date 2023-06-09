// Fill out your copyright notice in the Description page of Project Settings.


#include "Glass.h"
#include "Components/BoxComponent.h"

// Sets default values
AGlass::AGlass()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Glass = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Glass"));
	Glass->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(Glass);
}

// Called when the game starts or when spawned
void AGlass::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AGlass::OnBeginOverlap);
}

// Called every frame
void AGlass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGlass::GlassBreak()
{
	Server_GlassBreak();
}

void AGlass::SetBreakSoundAttenuation(float MaxDistance)
{
	if (Sound)
	{
		USoundAttenuation* AttenuationSettings = NewObject<USoundAttenuation>(this, USoundAttenuation::StaticClass());
		AttenuationSettings->Attenuation.FalloffDistance = MaxDistance;
		Sound->AttenuationSettings = AttenuationSettings;
	}
}

void AGlass::Server_GlassBreak_Implementation()
{
	NetMulticast_GlassBreak();
}

bool AGlass::Server_GlassBreak_Validate()
{
	return true;
}

void AGlass::NetMulticast_GlassBreak_Implementation()
{
	SetBreakSoundAttenuation(1000.f);
	UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, Sound, GetActorLocation());
}

void AGlass::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GlassBreak();
	Destroy();
	
}

void AGlass::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGlass, Sound);
}

