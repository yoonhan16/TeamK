// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingPlayerState_CPP.h"
#include <Net/UnrealNetwork.h>
#include "SystemChar.h"

void AShootingPlayerState_CPP::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(AShootingPlayerState_CPP, CurHP_CPP, COND_None, REPNOTIFY_Always);

}

AShootingPlayerState_CPP::AShootingPlayerState_CPP()
{

}


