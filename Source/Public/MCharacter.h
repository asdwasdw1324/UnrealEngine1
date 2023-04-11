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
	AMCharacter();//声明自身类的构造函数

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")//声明指向USpringArmComponent类的指针，名为SpringArmComp，放置在Camera的菜单下（注意include类的头文件）
	class USpringArmComponent *SpringArmComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")//声明指向CameraComponent类的指针，名为CameraComp，放置在Camera的菜单下（注意include类的头文件）
	class UCameraComponent  *CameraComp; 

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//声明4个移动控制函数，分别为向前，向右，开始跳跃，停止跳跃
	UFUNCTION()
	void MoveForward(float value);

	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	// 声明发射物射击的函数。
	UFUNCTION()
	void Fire();

	//声明一个指向AMyProjectile类的子类的指针，名为ProjectileClass
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AMyProjectile> ProjectileClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 声明枪口相对于摄像机位置的偏移的三维向量变量，该三维向量变量确定子弹发射位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;
};