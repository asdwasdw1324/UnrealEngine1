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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//ָ������ָ��AActor���ָ�룬�ֱ���Camera1��Camera2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Configuration")
	AActor* CameraOne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Configuration")
	AActor* CameraTwo;

	//����һ�������Ͳ�������Ϊ����Camera�л��ļ��ʱ�䣬�������Tick�����������DeltaTime���ϼ�Сֱ��С��0��������ã�����ٲ��ϼ�С
	float TimeToNextCameraChange;

	//UPROPERTY(EditAnywhere, Category = "Camera Configuration")
	//TArray<AActor*> CameraList;

	//��ǰ����Camera���
	int CurrentCameraIndex; // ����Ϊint32����
};
