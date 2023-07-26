// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyProjectile.h"
#include "SInteractionComponent.h"
#include "MCharacter.generated.h"

UCLASS()
class UNREALENGINE1_API AMCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	//����������Ĺ��캯��
	AMCharacter();

protected:

	//����ָ��USpringArmComponent���ָ�룬��ΪSpringArmComp��������Camera�Ĳ˵��£�ע��include���ͷ�ļ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent *SpringArmComp;
	
	//����ָ��UCameraComponent���ָ�룬��ΪCameraComp��������Camera�Ĳ˵��£�ע��include���ͷ�ļ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent  *CameraComp; 

	//����ָ��USInteractionComponent���ָ�룬��ΪInteraction��ע��include���ͷ�ļ���
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	class USInteractionComponent* InteractionComp;

	//������������ӽǵĲ�������
	UPROPERTY(EditAnywhere, Category = "Camera")
	bool bFreeCameraMode = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//����4���ƶ����ƺ������ֱ�Ϊ��ǰ�����ң���ʼ��Ծ��ֹͣ��Ծ
	UFUNCTION()
	void MoveForward(float value);

	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	UFUNCTION()
	void ToggleFreeCameraModeFree();

	UFUNCTION()
	void ToggleFreeCameraModeLock();

	UFUNCTION()
	void PrimaryInteract();

	UFUNCTION()
	void Fire();

	FTimerHandle TimerHandle_Fire;

	//����һ��ָ��AMyProjectile��������ָ�룬��ΪProjectileClass1/2/3
	//AActor---AMyProjectile---ProjectileClass1/2/3
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	TSubclassOf<class AMyProjectile> ProjectileClass1;
	
	//UPROPERTY(EditDefaultsOnly, Category = Attack)
	//TSubclassOf<class AMyProjectile> ProjectileClass2;
	
	//UPROPERTY(EditDefaultsOnly, Category = Attack)
	//TSubclassOf<class AMyProjectile> ProjectileClass3;

	UPROPERTY(EditAnywhere, Category = Attack)
	class UAnimMontage* AttackAnim;;

	void Fire_TimeElapsed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ����ǹ������������λ�õ�ƫ�Ƶ���ά��������������ά��������ȷ���ӵ�����λ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;
};