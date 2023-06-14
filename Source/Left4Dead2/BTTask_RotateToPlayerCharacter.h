// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BasicZombie.h"
#include "BTTask_RotateToPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class LEFT4DEAD2_API UBTTask_RotateToPlayerCharacter : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTTask_RotateToPlayerCharacter(FObjectInitializer const& object_initializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;


};
