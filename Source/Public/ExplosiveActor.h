// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveActor.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;
class UMaterialInstanceDynamic;

UCLASS()
class UNREALENGINE1_API AExplosiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ForceComp;

	//UPROPERTY(EditDefaultsOnly, Category = Materia)
	//UMaterialInstanceDynamic* ExplosiveActorMaterialInstance;

	UFUNCTION(BlueprintCallable)
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
