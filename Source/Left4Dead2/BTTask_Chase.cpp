// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Chase.h"
#include "AIController_CPP.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BlackBoard_Keys.h"

UBTTask_Chase::UBTTask_Chase(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("BTTTask_Chase_CPP");
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// AI ��Ʈ�ѷ��� ���� TargetLocation ��������
	AAIController_CPP* const Controller = Cast<AAIController_CPP>(owner_comp.GetAIOwner());
	FVector const player_location = Controller->get_blackboard()->GetValueAsVector(BB_Keys::Target_Location);

	// ĳ���� ��ġ�� �̵�
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, player_location);

	// �������� ������

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
