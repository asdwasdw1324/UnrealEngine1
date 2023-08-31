// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChange, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALENGINE1_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float delta);

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnHealthChange OnHealthChange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;
};
