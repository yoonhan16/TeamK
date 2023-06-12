// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AIController_CPP.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BlackBoard_Keys.h"


UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("FindPlayerLocation_CPP");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{

	// �÷��̾� ĳ���� & AI Controller �������� 
	ACharacter* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto const Controller = Cast<AAIController_CPP>(owner_comp.GetAIOwner());
	auto const Enemy = Controller->GetCharacter();

	if (Enemy && Enemy->IsA<ABasicZombie>())
	{
		BasicZombie = Cast<ABasicZombie>(Enemy);
		BasicZombie->UpdateSpeed(700.0f);
	}


	// �÷��̾� ĳ������ ��ġ ��������
	FVector const player_location = PlayerCharacter->GetActorLocation();
	if (search_random)
	{
		FNavLocation NavLocation;

		// ������̼� �ý��� �������� & ����޽� ���� �÷��̾� ĳ���� ��ó�� �̵�
		UNavigationSystemV1* const nav_system = UNavigationSystemV1::GetCurrent(GetWorld());
		if (nav_system->GetRandomPointInNavigableRadius(player_location, search_radius, NavLocation, nullptr))
		{
			Controller->get_blackboard()->SetValueAsVector(BB_Keys::Target_Location, NavLocation.Location);
		}

	}
	else
	{
		Controller->get_blackboard()->SetValueAsVector(BB_Keys::Target_Location, player_location);
	}

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
