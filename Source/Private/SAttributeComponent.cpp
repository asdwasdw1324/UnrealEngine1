// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...

	MaxHealth = 100.0f;
	Health = MaxHealth;

	SetIsReplicatedByDefault(true);
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActors)
{
	if (FromActors)
	{
		return FromActors->FindComponentByClass<USAttributeComponent>();
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);

	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

bool USAttributeComponent::ApplyHealthChange(float delta)
{
	float OldHealth = Health;

	Health = FMath::Clamp(Health + delta, 0.0f, MaxHealth);
	
	float ActualDelta = Health - OldHealth;
	OnHealthChange.Broadcast(nullptr, this, Health, ActualDelta);

	return ActualDelta != 0;
}

bool USAttributeComponent::IsAlive() const
{
	if (Health >= 0)
	{
		return true;
	}
	return false;
}

