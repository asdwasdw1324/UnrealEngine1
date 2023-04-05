// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealEngine1GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UNREALENGINE1_API AUnrealEngine1GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
};
