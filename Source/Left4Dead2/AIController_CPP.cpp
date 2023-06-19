// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_CPP.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ShootingPlayerState_CPP.h"
#include "SystemChar.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "BlackBoard_Keys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"


AAIController_CPP::AAIController_CPP(FObjectInitializer const& object_initializer)
{
	// BehaviorTree 하드코딩
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> Tree_obj(TEXT("BehaviorTree'/Game/TEAM_K/Left4Dead/KIM_YoonHan/Behaviour_Tree/BehaviorTree_BasicZombie.BehaviorTree_BasicZombie'"));

	if (Tree_obj.Succeeded())
	{
		bTree = Tree_obj.Object;
	}
	behavior_tree_component = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	// 시각 센서 사용
	setup_perception_system();
}

void AAIController_CPP::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(bTree);
	behavior_tree_component->StartTree(*bTree);
}

void AAIController_CPP::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	if (blackboard)
	{
		// BehaviorTree가 가진 Blackboard 실행
		blackboard->InitializeBlackboard(*bTree->BlackboardAsset);
	}
}
// cpp 내에서 블랙보드 키값 설정을 위한 함수
UBlackboardComponent* AAIController_CPP::get_blackboard() const
{
	return blackboard;
}

void AAIController_CPP::on_target_detected(AActor* actor, FAIStimulus const stimulus)
{
	// Actor 가 ASystemChar 클래스인지 확인
	if (actor && actor->IsA<ASystemChar>())
	{
		
		get_blackboard()->SetValueAsBool(BB_Keys::can_see_player, stimulus.WasSuccessfullySensed());

		//PlayerCharacter = Cast<ASystemChar>(actor);
		//PlayerState_CPP = PlayerCharacter->GetPlayerState<AShootingPlayerState_CPP>();

		//if (PlayerState_CPP && !PlayerState_CPP->Player_IsDead)
		//{
		//	get_blackboard()->SetValueAsBool(BB_Keys::can_see_player, stimulus.WasSuccessfullySensed());
		//	get_blackboard()->SetValueAsBool(BB_Keys::IsDead, false);
		//}
		//else
		//{
		//	get_blackboard()->SetValueAsBool(BB_Keys::can_see_player, false);
		//	get_blackboard()->SetValueAsBool(BB_Keys::IsDead, true);
		//}
	}
	//else
	//{
	//	get_blackboard()->SetValueAsBool(BB_Keys::can_see_player, false);
	//}

}

void AAIController_CPP::setup_perception_system()
{
	// 시각 센서 세팅
	sight_config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	sight_config->SightRadius = 1800.0f;
	sight_config->LoseSightRadius = sight_config->SightRadius + 200.0f;
	sight_config->PeripheralVisionAngleDegrees = 70.0f;
	sight_config->SetMaxAge(5.0f);
	sight_config->AutoSuccessRangeFromLastSeenLocation = 1500.0f;
	sight_config->DetectionByAffiliation.bDetectEnemies = true;
	sight_config->DetectionByAffiliation.bDetectNeutrals = true;
	sight_config->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->SetDominantSense(*sight_config->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_CPP::on_target_detected);
	GetPerceptionComponent()->ConfigureSense(*sight_config);

}
