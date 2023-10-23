// Fill out your copyright notice in the Description page of Project Settings.


#include "MPowerupActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMPowerupActor::AMPowerupActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("PowerupActor");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;
	bIsActive = true;

	// Directly set bool instead of going through SetReplicates(true) within constructor,
	// Only use SetReplicates() outside constructor
	bReplicates = true;
}


void AMPowerupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ensure(MeshComp))
	{
		FVector NewLocation = GetActorLocation();
		FRotator NewRotation = GetActorRotation();
		float RunningTime = GetGameTimeSinceCreation();
		float DeltaHeight = (FMath::Cos(RunningTime + DeltaTime) - FMath::Cos(RunningTime));
		NewLocation.Z += DeltaHeight * 40.0f;
		float DeltaRotation = DeltaTime * 40.0f;
		NewRotation.Yaw += DeltaRotation;
		SetActorLocationAndRotation(NewLocation, NewRotation);
	}

}

void AMPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in derived classes...
}


void AMPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}


void AMPowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AMPowerupActor::ShowPowerup, RespawnTime);
}

void AMPowerupActor::SetPowerupState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();
}


void AMPowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	// Set visibility on root and all children
	RootComponent->SetVisibility(bIsActive, true);
}

void AMPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMPowerupActor, bIsActive);
}

