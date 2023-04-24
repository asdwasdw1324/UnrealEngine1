// Copyright Epic Games, Inc. All Rights Reserved.


#include "UnrealEngine1GameModeBase.h"
#include "Engine/Engine.h"

void AUnrealEngine1GameModeBase::BeginPlay() {
	Super::BeginPlay();
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using TPSCharacter!"));//游戏开始时在屏幕上打印相关字符串，并设置其颜色为红色，停留5秒
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is MyNewGameModeBase!"));

}