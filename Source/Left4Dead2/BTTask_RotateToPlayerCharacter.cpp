// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RotateToPlayerCharacter.h"
#include "AIController_CPP.h"
#include "ShootingPlayerState_CPP.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BlackBoard_Keys.h"


UBTTask_RotateToPlayerCharacter::UBTTask_RotateToPlayerCharacter(FObjectInitializer const& object_initializer)
{
	// 노드 이름 설정 & 추가로 설정할 블랙보드 키의 필터 설정
	NodeName = TEXT("RotateToPlayerCharacter_CPP");
	BlackboardKey1.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RotateToPlayerCharacter, BlackboardKey1));
	BlackboardKey2.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RotateToPlayerCharacter, BlackboardKey2));
}

EBTNodeResult::Type UBTTask_RotateToPlayerCharacter::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{

	auto const Controller = Cast<AAIController_CPP>(owner_comp.GetAIOwner());
	auto const Enemy = Controller->GetCharacter();
	FVector const zombie_location = Enemy->GetActorLocation();

	double MinDistance = 0;
	int32 NearestPlayerIndex = -1;

	for (int32 i = 0; i < 4; i++)
	{
		// 가장 가까운 플레이어 위치 & 인덱스 찾기
		ACharacter* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), i);
		
		if (PlayerCharacter)
		{
			AShootingPlayerState_CPP* PlayerState = PlayerCharacter->GetPlayerState<AShootingPlayerState_CPP>();

			if (PlayerState && PlayerState->Player_IsDead == false)
			{
				double CurrentDistance = FVector::Dist(PlayerCharacter->GetActorLocation(), zombie_location);

				if (i == 0)
				{
					MinDistance = CurrentDistance;
					TargetCharacter = PlayerCharacter;
					NearestPlayerIndex = i;
				}
				else
				{
					if (CurrentDistance < MinDistance)
					{
						TargetCharacter = PlayerCharacter;
						NearestPlayerIndex = i;
					}
				}
			}
			//else
			//{
			//	FinishLatentTask(owner_comp, EBTNodeResult::Failed);
			//	return EBTNodeResult::Failed;
			//}
		}
	}

	if (TargetCharacter)
	{
		// 가장 가까운 플레이어 & 플레이어의 방향 & 플레이어 인덱스 추출
		Controller->get_blackboard()->SetValueAsObject(BB_Keys::Nearest_Player, TargetCharacter);
		FVector const player_location = TargetCharacter->GetActorLocation();
		FVector const player_direction = TargetCharacter->GetActorLocation() - zombie_location;
		Controller->get_blackboard()->SetValueAsVector(BB_Keys::Player_Location, player_direction);
		Controller->get_blackboard()->SetValueAsInt(BB_Keys::Nearest_Index, NearestPlayerIndex);
	}
	else
	{
		// 블랙보드 키 데이터 초기화
		Controller->get_blackboard()->ClearValue(BB_Keys::Nearest_Player);
		Controller->get_blackboard()->ClearValue(BB_Keys::Player_Location);
		Controller->get_blackboard()->ClearValue(BB_Keys::Nearest_Index);
	}

	// 성공으로 Task 종료
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
