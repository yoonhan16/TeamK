// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Math/UnrealMathUtility.h"
#include "InteractInterface.h"
#include "Door.generated.h"

UCLASS()
class LEFT4DEAD2_API ADoor : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor();

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

protected:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* DoorFrame;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", Replicated)
	UStaticMeshComponent* Door;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* BoxCollision;

	UFUNCTION(Server, Reliable)
	void Server_OpenDoor();
	virtual void Server_OpenDoor_Implementation();
	virtual bool Server_OpenDoor_Validate();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OpenDoor();
	virtual void Multicast_OpenDoor_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
	UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere)
	float DoorRotateAngle = 90.f;

	UPROPERTY(ReplicatedUsing = OnRep_IsDoorClosed)
	bool bIsDoorClosed = true;

	UFUNCTION()
	void OnRep_IsDoorClosed();

	UFUNCTION()
	void ControlDoor(float Value);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
