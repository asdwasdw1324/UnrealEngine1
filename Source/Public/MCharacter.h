// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class UAnimMontage;
class AMyProjectile;
class USAttributeComponent;
class AProjectileBase;
class ADashProjectile;

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
	USpringArmComponent *SpringArmComp;
	
	//����ָ��UCameraComponent���ָ�룬��ΪCameraComp��������Camera�Ĳ˵��£�ע��include���ͷ�ļ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent *CameraComp; 

	//����ָ��USInteractionComponent���ָ�룬��ΪInteraction��ע��include���ͷ�ļ���
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	USInteractionComponent* InteractionComp;

	//����ָ�򿪻����ʱ�Ķ���ָ��
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;;

	//����ָ��Ѫ���仯�����ָ��
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
	USAttributeComponent* AttributeComp;

	//������������ӽǵĲ�������
	UPROPERTY(EditAnywhere, Category = "CameraConfiguration")
	bool bFreeCameraMode = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//�������ƺ������ֱ�Ϊ��ǰ�����ң���ʼ��Ծ��ֹͣ��Ծ
	UFUNCTION()
	void MoveForward(float value);

	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void ToggleFreeCameraModeFree();

	UFUNCTION()
	void ToggleFreeCameraModeLock();

	UFUNCTION()
	void PrimaryInteract();

	UFUNCTION()
	void Fire();

	//�����ӳټ�ʱ�����
	FTimerHandle TimerHandle_Fire;

	//����һ��ָ��AMyProjectile��������ָ�룬��ΪProjectileClass1/2/3
	//�̳��߼�ΪAActor->AMyProjectile->ProjectileClass1
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	TSubclassOf<AMyProjectile> ProjectileClass1;

	UPROPERTY(EditDefaultsOnly, Category = Attack)
	TSubclassOf<AProjectileBase> ProjectileClass2;

	UPROPERTY(EditDefaultsOnly, Category = Attack)
	TSubclassOf<ADashProjectile> ProjectileClass3;
	
	//�ӳ���������ú������ڲ��߼����������豩¶����ͼʹ��
	void Fire_TimeElapsed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ����ǹ������������λ�õ�ƫ�Ƶ���ά��������������ά��������ȷ���ӵ�����λ�ã��ڵ�ǰSpawn����ȷ�������ʱ����Ҫ�ñ�����
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	//FVector MuzzleOffset;
};