// Fill out your copyright notice in the Description page of Project Settings.


#include "SandBoxPawn.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSandBoxPawn, All, All)

// Sets default values
ASandBoxPawn::ASandBoxPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticComp"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(StaticMeshComponent);
	SpringArmComponent->TargetArmLength = 300.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 4.0f;
	SpringArmComponent->SetRelativeLocation(FVector(-146.0f, 0.0f, 130.0f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);

}

void ASandBoxPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//UE_LOG(LogSandBoxPawn, Error, TEXT("%s possessed %s"), *GetName(), *NewController->GetName());
}

void ASandBoxPawn::UnPossessed()
{
	Super::UnPossessed();

	//UE_LOG(LogSandBoxPawn, Warning, TEXT("%s possessed"), *GetName());
}

// Called when the game starts or when spawned
void ASandBoxPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASandBoxPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CustomMove(DeltaTime, Velocity, VelocityVector);

}

// Called to bind functionality to input
void ASandBoxPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASandBoxPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASandBoxPawn::MoveRight);
}

void ASandBoxPawn::MoveForward(float value)
{
	UE_LOG(LogSandBoxPawn, Display, TEXT("MoveForward value is: %f"), value);

	VelocityVector.X = value;

	
}

void ASandBoxPawn::MoveRight(float value)
{
	UE_LOG(LogSandBoxPawn, Display, TEXT("MoveRight value is: %f"), value);

	VelocityVector.Y = value;

	
}

void ASandBoxPawn::CustomMove(float deltatime, float velocity, FVector velocityvector)
{
	if (!velocityvector.IsZero())
	{
		const FVector NewLocation = GetActorLocation() + velocity * deltatime * velocityvector;
		SetActorLocation(NewLocation);
	}
}

