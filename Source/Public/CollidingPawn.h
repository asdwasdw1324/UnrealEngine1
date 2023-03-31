// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CollidingPawn.generated.h"

UCLASS()
class UNREALENGINE1_API ACollidingPawn : public APawn  //����һ��ACollidingPawn�࣬�̳���APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollidingPawn();

	//����һ��ָ������ϵͳ������ָ��
	UPROPERTY()
	class UParticleSystemComponent* OurParticleSystem;

	//����һ��ָ����ײ�����ƶ�������ָ��,��������ײ�ƶ����Դ�ļ���ͨ���̳�UPawnMovementComponent�����д��TickComponent����
	UPROPERTY()
	class UCollidingPawnMovementComponent* OurMovementComponent;

	//���ص�ǰ Pawn ���ƶ������MovementComponent�������ڿ��� Pawn ���˶�
	virtual UPawnMovementComponent* GetMovementComponent() const override;//UPawnMovementComponent* MyMovementComponent = GetMovementComponent();

	//����4���ƶ��Ͳ�������
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
