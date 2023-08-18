// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class UNREALENGINE1_API ASItemChest : public AActor, public IMyInterface //必须继承自某个API，才能调用API下的某个函数
{
	GENERATED_BODY()

public:
	//宝箱打开角度设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParaMeterList)
	float TargetPitch;
	
	//定义继承自IMyInterface的Interact函数方法的具体实现方法
	void Interact_Implementation(APawn* InstigatorPawn);

public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//定义指向宝箱盒子和盖子的UStaticMeshComponent指针
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TreasureObj)
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TreasureObj)
	UStaticMeshComponent* LidMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
