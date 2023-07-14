// Copyright Epic Games, Inc. All Rights Reserved.


#include "UnrealEngine1GameModeBase.h"
#include "Engine/Engine.h"
#include "SandBoxPawn.h"
#include "SandBoxPlayerController.h"

AUnrealEngine1GameModeBase::AUnrealEngine1GameModeBase()
{
	DefaultPawnClass = ASandBoxPawn::StaticClass();
	PlayerControllerClass = ASandBoxPlayerController::StaticClass();
}

void AUnrealEngine1GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	//游戏开始时在屏幕上打印相关字符串，并设置其颜色为红色，停留5秒
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("We are using TPSCharacter!"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is MyNewGameModeBase!"));

}