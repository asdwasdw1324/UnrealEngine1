// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyProjectile.h"
#include "MCharacter.generated.h"

UCLASS()
class UNREALENGINE1_API AMCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMCharacter();//����������Ĺ��캯��

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")//����ָ��USpringArmComponent���ָ�룬��ΪSpringArmComp��������Camera�Ĳ˵��£�ע��include���ͷ�ļ���
	class USpringArmComponent *SpringArmComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")//����ָ��CameraComponent���ָ�룬��ΪCameraComp��������Camera�Ĳ˵��£�ע��include���ͷ�ļ���
	class UCameraComponent  *CameraComp; 

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

	// ��������������ĺ�����
	UFUNCTION()
	void Fire();

	//����һ��ָ��AMyProjectile��������ָ�룬��ΪProjectileClass
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AMyProjectile> ProjectileClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ����ǹ������������λ�õ�ƫ�Ƶ���ά��������������ά��������ȷ���ӵ�����λ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;
};