// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AMAIController::BeginPlay()
{

	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this,0);

	FString MyPawnName = GetNameSafe(MyPawn);
	UE_LOG(LogTemp, Warning, TEXT("The Target Location is: %s"), *MyPawnName);

	if (MyPawn)
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("CharacterLocation", MyPawn);
	}

}

void AMAIController::Tick(float DeltaTime)
{

	//Super::Tick(DeltaTime);
	//APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//if (MyPawn)
	//{
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
	//}

}
