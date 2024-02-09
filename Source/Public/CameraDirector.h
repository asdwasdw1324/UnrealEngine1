// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraDirector.generated.h"

UCLASS()
class UNREALENGINE1_API ACameraDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraDirector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//指定两个指向AActor类的指针，分别是Camera1和Camera2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera | Camera Configuration")
	AActor* CameraOne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera | Camera Configuration")
	AActor* CameraTwo;

	//定义一个浮点型参数，作为两个Camera切换的间隔时间，其会随着Tick函数计算出的DeltaTime不断减小直至小于0，随后被重置，随后再不断减小
	float TimeToNextCameraChange;

	UPROPERTY(EditAnywhere, Category = "Camera | Camera Configuration")
	TArray<AActor*> CameraDropdownList;

	//当前激活Camera序号
	int32 CurrentCameraIndex; // 声明为int32类型
};
