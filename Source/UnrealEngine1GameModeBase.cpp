// Copyright Epic Games, Inc. All Rights Reserved.


#include "UnrealEngine1GameModeBase.h"
#include "Engine/Engine.h"

void AUnrealEngine1GameModeBase::BeginPlay() {
	Super::BeginPlay();
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using TPSCharacter!"));//��Ϸ��ʼʱ����Ļ�ϴ�ӡ����ַ���������������ɫΪ��ɫ��ͣ��5��
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is MyNewGameModeBase!"));

}