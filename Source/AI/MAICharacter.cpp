// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MAICharacter.h"

// Sets default values
AMAICharacter::AMAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

