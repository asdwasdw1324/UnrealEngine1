// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInterface.h"
#include "MPowerupActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class UNREALENGINE1_API AMPowerupActor : public AActor, public IMyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMPowerupActor();

protected:

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();

	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

	UFUNCTION()
	void ShowPowerup();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	virtual void Tick(float DeltaTime) override;

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

	
};
