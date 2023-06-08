// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInterface.h"
#include "SItemChest.generated.h"

UCLASS()
class UNREALENGINE1_API ASItemChest : public AActor, public IMyInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = ParaMeterList)
	float targetpitch;
	
	void Interact_Implementation(APawn* InstigatorPawn);

public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TreasureObj)
	class UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TreasureObj)
	class UStaticMeshComponent* LidMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
