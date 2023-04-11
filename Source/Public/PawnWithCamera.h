// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PawnWithCamera.generated.h"

UCLASS()
class UNREALENGINE1_API APawnWithCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnWithCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "SpringArm Configuration")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, Category = "Camera Configuration")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, Category = "StaticMesh Configuration")
	class UStaticMeshComponent* StaticMeshComp;

	//输入变量
	FVector2D MovementInput;//2D向量变量
	FVector2D CameraInput;//2D向量变量
	float ZoomFactor = 0;//放大系数
	bool bZoomingIn;//是否放缩判断

	//输入函数
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void ZoomIn();
	void ZoomOut();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
