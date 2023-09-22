// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MAICharacter.h"
#include "Perception\PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMAICharacter::AMAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
}

void AMAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AMAICharacter::OnPawnSeen);
}

void AMAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
		BBComp->SetValueAsObject("CharacterLocation", Pawn);
		DrawDebugString(GetWorld(), GetActorLocation(), TEXT("Player Spotted"), nullptr, FColor::Red, 3.0f, true, 2.0f);
	}
}
