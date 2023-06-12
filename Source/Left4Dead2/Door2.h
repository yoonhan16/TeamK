// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Math/UnrealMathUtility.h"
#include "InteractInterface.h"
#include "Door2.generated.h"

UCLASS()
class LEFT4DEAD2_API ADoor2 : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void MyInteract_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Door")
	void OpenDoor();

	UPROPERTY(EditAnywhere, Category = "Door")
	class UTimelineComponent* DoorTimeline;

private:


protected:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* BoxCollision;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* DoorFrame;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", Replicated)
	UStaticMeshComponent* LeftDoor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", Replicated)
	UStaticMeshComponent* RightDoor;

	UFUNCTION(Server, Reliable)
	void Server_OpenDoor();
	void Server_OpenDoor_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OpenDoor();
	void Multicast_OpenDoor_Implementation();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	FTimeline Timeline;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UCurveFloat* CurveFloat;

	bool bIsDoorClosed = true;

	UPROPERTY(EditAnywhere)
	float DoorRotateAngle = 90.f;

	bool bDoorOnSameSide;
	//void SetDoorOnSameSide();

	UFUNCTION()
	void ControlDoor(float Value);
};
