// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class UNREALENGINE1_API ASItemChest : public AActor, public IMyInterface //����̳���ĳ��API�����ܵ���API�µ�ĳ������
{
	GENERATED_BODY()

public:
	//����򿪽Ƕ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParaMeterList)
	float TargetPitch;
	
	//����̳���IMyInterface��Interact���������ľ���ʵ�ַ���
	void Interact_Implementation(APawn* InstigatorPawn);

public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:
	//����ָ������Ӻ͸��ӵ�UStaticMeshComponentָ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TreasureObj)
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TreasureObj)
	UStaticMeshComponent* LidMesh;

};
