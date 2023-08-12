// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
//class UMaterialInstanceDynamic;

UCLASS()
class UNREALENGINE1_API AMyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//������ײ���
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	//�������ƶ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	UParticleSystemComponent* ParticleSystemComponent;

	// ��ʼ����������Ϸ������ٶȵĺ�����
	void FireInDirection(const FVector& ShootDirection);

	// ������������
	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	//UStaticMeshComponent* ProjectileMeshComponent;

	// ���������
	//UPROPERTY(VisibleDefaultsOnly, Category = Materia)
	//UMaterialInstanceDynamic* ProjectileMaterialInstance;

	virtual void PostInitializeComponents() override;

	// ���������������ʱ����õĺ�����
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
