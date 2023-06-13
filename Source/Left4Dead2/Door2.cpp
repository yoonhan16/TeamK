// Fill out your copyright notice in the Description page of Project Settings.


#include "Door2.h"
#include "Components/BoxComponent.h"

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

	// 네트워크 복제 기능 활성화
	bReplicates = true;
	SetReplicateMovement(false);
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void ADoor2::BeginPlay()
{
	Super::BeginPlay();

	// CurveFloat이 유효한 경우 ControlDoor 함수를 타임라인의 float 진행 이벤트에 바인딩
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("ControlDoor"));
		DoorTimeline->AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

// Called every frame
void ADoor2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 액터의 로컬 역할을 디버그 문자열로 그려줌
	FString value;
	UEnum::GetValueAsString(GetLocalRole(), value);
	DrawDebugString(GetWorld(), FVector(0, 0, 100), value, this, FColor::Green, DeltaTime);
}

void ADoor2::MyInteract_Implementation()
{
	// 서버에서 함수가 호출되는지 확인
	if (ROLE_Authority)
	{
		OpenDoor();
	}
}

// Server_OpenDoor 함수 호출
void ADoor2::OpenDoor()
{
	Server_OpenDoor();
	UE_LOG(LogTemp, Warning, TEXT("Interacted with Door!"));
}

// MulticastSyncDoorState 함수 호출하여 현재 문 상태 동기화
void ADoor2::Server_OpenDoor_Implementation()
{
	MulticastSyncDoorState(bIsOpen);
}

// Server_OpenDoor 함수의 유효성을 검증
// 클라이언트가 해당 함수를 호출할 권한이 있는지 검증하고, true를 반환
bool ADoor2::Server_OpenDoor_Validate()
{
	return true;
}

// 문 상태 업데이트 및 상태에 따라 타임라인 재생/역재생
void ADoor2::MulticastSyncDoorState_Implementation(bool bNewDoorState)
{
	bIsOpen = bNewDoorState;
	if (bIsOpen)
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

	// 문 상태 토글
	bIsOpen = !bIsOpen;
}

// bIsOpen 변수가 서버에서 변경되었을 때 MulticastSyncDoorState 함수 호출
void ADoor2::OnRep_bIsOpen()
{
	MulticastSyncDoorState(bIsOpen);
}

// 부모 클래스의 GetLifetimeReplicatedProps 함수를 호출하여 기본 복제 설정을 가져옴
void ADoor2::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// LeftDoor, RightDoor와 bIsOpen 변수 복제 설정
	// DOREPLIFETIME 매크로는 변수와 해당 변수의 상태를 복제하도록 지정하는 역할
	// 복제 설정을 추가하면 서버와 클라이언트 간에 해당 변수의 값과 상태가 동기화됨
	DOREPLIFETIME(ADoor2, LeftDoor);
	DOREPLIFETIME(ADoor2, RightDoor);
	DOREPLIFETIME_CONDITION(ADoor2, bIsOpen, COND_SkipOwner);
	// COND_SkipOwner 조건을 지정하여 복제할 때 소유자를 제외한 다른 모든 클라이언트에게 동기화
}

void ADoor2::ControlDoor(float Value)
{
	// 문의 초기 회전값과 목표 회전값 정의
	FRotator DoorInitialRotation1 = FRotator(0.f, 0.f, 0.f);
	FRotator DoorInitialRotation2 = FRotator(0.f, -180.f, 0.f);
	FRotator DoorTargetRoatation1 = FRotator(0.f, DoorRotateAngle, 0.f);
	FRotator DoorTargetRoatation2 = FRotator(0.f, -180.f -DoorRotateAngle, 0.f);

	// Value 매개변수에 기반하여 초기 회전값과 목표 회전값 사이를 보간
	FRotator Rot1 = FMath::Lerp(DoorInitialRotation1, DoorTargetRoatation1, Value);
	FRotator Rot2 = FMath::Lerp(DoorInitialRotation2, DoorTargetRoatation2, Value);

	// Door 컴포넌트의 상대 회전값 설정
	LeftDoor->SetRelativeRotation(Rot1);
	RightDoor->SetRelativeRotation(Rot2);
}