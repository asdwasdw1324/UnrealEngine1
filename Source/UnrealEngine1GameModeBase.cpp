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
	
	//��Ϸ��ʼʱ����Ļ�ϴ�ӡ����ַ���������������ɫΪ��ɫ��ͣ��5��
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("We are using TPSCharacter!"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is MyNewGameModeBase!"));

}