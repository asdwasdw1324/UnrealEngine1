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

// EditAnywhere - edit in BP editor and per-instance in level.
// VisibleAnywhere - 'read-only' in editor and level. (Use for Components)
// EditDefaultsOnly - hide variable per-instance, edit in BP editor only
// VisibleDefaultsOnly - 'read-only' access for variable, only in BP editor (uncommon)
// EditInstanceOnly - allow only editing of instance (eg. when placed in level)
// --
// BlueprintReadOnly - read-only in the Blueprint scripting (does not affect 'details'-panel)
// BlueprintReadWrite - read-write access in Blueprints
// --
// Category = "" - display only for detail panels and blueprint context menu.

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActors);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (Displayname = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnHealthChange OnHealthChange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth;
};
