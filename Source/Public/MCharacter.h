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
	AMCharacter();

protected:

	//声明指向USpringArmComponent类的指针，名为SpringArmComp，放置在Camera的菜单下（注意include类的头文件）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent *SpringArmComp;
	
	//声明指向UCameraComponent类的指针，名为CameraComp，放置在Camera的菜单下（注意include类的头文件）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent *CameraComp; 

	//声明指向USInteractionComponent类的指针，名为InteractionComp（注意include类的头文件）
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	USInteractionComponent* InteractionComp;

	//声明指向开火射击时的动画指针
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnimComp;;

	//声明指向角色属性的组件指针
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Attributes")
	USAttributeComponent* AttributeComp;

	//声明自由相机视角的布尔变量
	UPROPERTY(EditAnywhere, Category = "Camera")
	bool bFreeCameraMode;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//声明各个控制操作函数
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

	UFUNCTION()
	void DashFire();

	//延迟射击所调用函数，内部逻辑函数，无需暴露给蓝图使用
	void Fire_TimeElapsed();

	//开火延迟计时器句柄
	FTimerHandle TimerHandle_Fire;

	//声明一个指向AMyProjectile类的子类的指针，名为ProjectileClass1
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	TSubclassOf<AMyProjectile> ProjectileClass1;

	//声明一个指向AProjectileBase类的子类的指针，名为ProjectileClass2
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	TSubclassOf<AProjectileBase> ProjectileClass2;

	//声明一个指向ADashProjectile类的子类的指针，名为ProjectileClass3
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	TSubclassOf<ADashProjectile> ProjectileClass3;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 声明枪口相对于摄像机位置的偏移的三维向量变量，该三维向量变量确定子弹发射位置（在当前Spawn方法确定发射点时不需要该变量）
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	//FVector MuzzleOffset;
};