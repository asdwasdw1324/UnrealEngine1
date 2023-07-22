// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CollidingPawn.generated.h"

UCLASS()
class UNREALENGINE1_API ACollidingPawn : public APawn  //声明一个ACollidingPawn类，继承自APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollidingPawn();

	//声明一个指向粒子系统组件类的指针
	UPROPERTY()
	class UParticleSystemComponent* OurParticleSystem;

	//声明一个指向碰撞棋子移动组件类的指针,该类在碰撞移动组件源文件中通过继承UPawnMovementComponent类后重写了TickComponent函数
	UPROPERTY()
	class UCollidingPawnMovementComponent* OurMovementComponent;

	//重写GetMovementComponent虚函数，返回值为指向UPawnMovementComponent类的指针
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	//声明4个移动和操作函数
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void ParticleToggle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InInputComponent) override;

};
