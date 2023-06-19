// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AIController_CPP.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BlackBoard_Keys.h"


UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& object_initializer)
{
	// 노드 이름 설정
	NodeName = TEXT("FindPlayerLocation_CPP");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// 플레이어 캐릭터 & AI Controller 가져오기 
	auto const Controller = Cast<AAIController_CPP>(owner_comp.GetAIOwner());
	auto const Enemy = Controller->GetCharacter();

	if (Enemy && Enemy->IsA<ABasicZombie>())
	{
		BasicZombie = Cast<ABasicZombie>(Enemy);
		BasicZombie->UpdateSpeed(700.0f);
	}
	// 가장 가까운 플레이어 가져오기
	ACharacter* TargetCharacter = (ACharacter*)Controller->get_blackboard()->GetValueAsObject(BB_Keys::Nearest_Player);

	// 플레이어 캐릭터의 위치 가져오기
	FVector const player_location = TargetCharacter->GetActorLocation();
	if (search_random)
	{
		FNavLocation NavLocation;

		// 월드에 배치된 내비메쉬 가저오기 & 내비메쉬 위의 플레이어 캐릭터 근처로 이동
		UNavigationSystemV1* const nav_system = UNavigationSystemV1::GetCurrent(GetWorld());

		// 플레이어가 시야 밖으로 벗어났을 때
		if (nav_system->GetRandomPointInNavigableRadius(player_location, search_radius, NavLocation, nullptr))
		{
			Controller->get_blackboard()->SetValueAsVector(BB_Keys::Target_Location, NavLocation.Location);
		}
	}
	else
	{
		// 플레이어가 시야 안에 있을 때
		Controller->get_blackboard()->SetValueAsVector(BB_Keys::Target_Location, player_location);
	}

	// 성공으로 Task 종료
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
