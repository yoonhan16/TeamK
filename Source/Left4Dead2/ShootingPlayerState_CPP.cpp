// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingPlayerState_CPP.h"
#include <Net/UnrealNetwork.h>
#include "AIController_CPP.h"
#include "SystemChar.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BlackBoard_Keys.h"


void AShootingPlayerState_CPP::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(AShootingPlayerState_CPP, CurHP_CPP, COND_None, REPNOTIFY_Always);
}

AShootingPlayerState_CPP::AShootingPlayerState_CPP()
{
	//Check_PlayerDeath();
}

void AShootingPlayerState_CPP::Server_SetCurHP_Implementation(float NewHP)
{
	CurHP_CPP = NewHP;
}

bool AShootingPlayerState_CPP::Server_SetCurHP_Validate(float NewHP)
{
	return true;
}

void AShootingPlayerState_CPP::SetCurHP(float NewHP)
{
	if (HasAuthority())
	{
		Server_SetCurHP(CurHP_CPP);
	}
	else
	{
		CurHP_CPP = NewHP;
	}
}

//void AShootingPlayerState_CPP::Check_PlayerDeath()
//{
//	Player_IsDead = (CurHP_CPP <= 0.0f);
//
//	auto Controller = Cast<AAIController_CPP>(GetOwner());
//
//	if (Player_IsDead)
//	{
//		Controller->get_blackboard()->SetValueAsBool(BB_Keys::IsDead, true);
//	}
//	else
//	{
//		Controller->get_blackboard()->SetValueAsBool(BB_Keys::IsDead, false);
//	}
//}





