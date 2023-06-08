// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FindRandomPatrolLocation.generated.h"

/**
 * 
 */
UCLASS()
class LEFT4DEAD2_API UBTT_FindRandomPatrolLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTT_FindRandomPatrolLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


};
