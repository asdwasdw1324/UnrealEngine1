// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "MyInterface.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectiveQueryParams;
	ObjectiveQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectiveQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	
	AActor* MyOwner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End = EyeLocation + (EyeRotation.Vector()*1000);

	//单射线碰撞检测，根据物体碰撞类型
	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectiveQueryParams);
	//AActor* HitActor = Hit.GetActor();
	//if (HitActor)
	//{
	//	Same operation with below
	//}
	
	//定义保存碰撞检测结果的TArray数组容器
	TArray<FHitResult> Hits;
	//定义碰撞检测组件的球体半径
	float Radius = 30.f;

	//定义碰撞组件并将其形状设置为Radius半径的球体
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	//返回碰撞布尔类型碰撞检测结果
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectiveQueryParams, Shape);

	//根据碰撞检测结果确定检测射线的颜色，真为绿，假为红
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	
	for (FHitResult Hit : Hits) 
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UMyInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				IMyInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor,false, 2.0f, 1.0f);
	}
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 2.0f);
}