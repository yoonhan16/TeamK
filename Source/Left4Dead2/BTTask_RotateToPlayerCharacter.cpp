// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RotateToPlayerCharacter.h"
#include "AIController_CPP.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BlackBoard_Keys.h"


UBTTask_RotateToPlayerCharacter::UBTTask_RotateToPlayerCharacter(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("RotateToPlayerCharacter_CPP");
}

EBTNodeResult::Type UBTTask_RotateToPlayerCharacter::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{

	auto const Controller = Cast<AAIController_CPP>(owner_comp.GetAIOwner());
	auto const Enemy = Controller->GetCharacter();
	FVector const zombie_location = Enemy->GetActorLocation();

	double MinDistance = 0;

	ACharacter* TargetCharacter = nullptr;

	for (int32 i = 0; i < 4; i++)
	{
		ACharacter* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), i);
		UE_LOG(LogTemp, Warning, TEXT("i: %d"), i);
		double CurrentDistance = FVector::Dist(PlayerCharacter->GetActorLocation(), zombie_location);

		if (i == 0)
		{
			MinDistance = CurrentDistance;
			TargetCharacter = PlayerCharacter;
		}
		else
		{
			if (CurrentDistance < MinDistance)
			{
				TargetCharacter = PlayerCharacter;
			}
		}
	}

	//ACharacter* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), *PlayerIndex);

	FVector const player_location = TargetCharacter->GetActorLocation();
	FVector const player_direction = TargetCharacter->GetActorLocation() - zombie_location;
	Controller->get_blackboard()->SetValueAsVector(BB_Keys::Player_Location, player_direction);

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
