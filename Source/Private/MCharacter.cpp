// Fill out your copyright notice in the Description page of Project Settings.


#include "MCharacter.h"
#include "GameFramework/characterMovementComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMCharacter::AMCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//初始化根组件,针对Character类会出错
	//RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultCharacterSceneComp");
	
	//为指针SpringArmComp指定USpringArmComponent的实例，命名为SpringArmComp并且附在RootComponent下
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	check(SpringArmComp != nullptr);
	SpringArmComp->SetupAttachment(RootComponent);
	//SpringArm默认参数设置
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 4.0f;

	//为指针CameraComp指定UCameraComponent的实例，命名为CameraComp并且附在SpringArmComp下
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	check(CameraComp != nullptr);
	CameraComp->SetupAttachment(SpringArmComp);

	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
}

// Called when the game starts or when spawned
void AMCharacter::BeginPlay()
{
	Super::BeginPlay();//继承BeginPlay函数
}

//4个移动操作函数的定义
void AMCharacter::MoveForward(float value) {
	//AddMovementInput(GetActorForwardVector(), value);
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, value);
}

void AMCharacter::MoveRight(float value) {
	//AddMovementInput(GetActorRightVector(), value);
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value);
}

void AMCharacter::StartJump()
{
	bPressedJump = true;
}

void AMCharacter::StopJump()
{
	bPressedJump = false;
}

void AMCharacter::ToggleFreeCameraModeFree() {
	bFreeCameraMode = true;
}

void AMCharacter::ToggleFreeCameraModeLock() {
	bFreeCameraMode = false;
}

// Called every frame
void AMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMCharacter::StopJump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMCharacter::Fire);
	PlayerInputComponent->BindAction("FreeCamera", IE_Pressed, this, &AMCharacter::ToggleFreeCameraModeFree);
	PlayerInputComponent->BindAction("FreeCamera", IE_Released, this, &AMCharacter::ToggleFreeCameraModeLock);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AMCharacter::PrimaryInteract);
}

void AMCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

//射击开火函数的定义
void AMCharacter::Fire()
{
	// 试图发射发射物。
	if (ProjectileClass1 || ProjectileClass2 || ProjectileClass3)
	{
		FVector MuzzleLocation = GetMesh()->GetSocketLocation("hand_r");
		FRotator MuzzleRotation = GetControlRotation();
		FTransform spawnTM = FTransform(MuzzleRotation, MuzzleLocation);
		
		// 获取摄像机的位置和旋转方向，将actoreyesviewpoint的location和rotation的值分别返回给两个参数
		//FVector CameraLocation;
		//FRotator CameraRotation;
		//GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// 设置MuzzleOffset，在略靠近摄像机前生成发射物。该坐标是一个相对位置坐标
		//MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// 将MuzzleOffset从摄像机空间变换到世界空间。
		//FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// 使目标方向略向上倾斜。
		//FRotator MuzzleRotation = CameraRotation;
		//MuzzleRotation.Pitch += 10.0f;
		//至此确定了发射物的位置和旋转方向，以MuzzleLocation和MuzzleRotation为具体参数

		PlayAnimMontage(AttackAnim);
		
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AMyProjectile* Projectile = nullptr;

			// 在枪口位置生成发射物,定义名为Projectile的指向AMyProjectile的指针变量
			if (ProjectileClass1) 
			{
				Projectile = World->SpawnActor<AMyProjectile>(ProjectileClass1, MuzzleLocation, MuzzleRotation, SpawnParams);
			}
			else if (ProjectileClass2)
			{
				Projectile = World->SpawnActor<AMyProjectile>(ProjectileClass2, MuzzleLocation, MuzzleRotation, SpawnParams);
			}
			else if (ProjectileClass3)
			{
				Projectile = World->SpawnActor<AMyProjectile>(ProjectileClass3, MuzzleLocation, MuzzleRotation, SpawnParams);
			}
			if (Projectile)
			{
				// 在生成发射物之后设置发射物的初始轨迹，调用FireInDirection函数
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
	else 
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Nothing to Fire!"));
	}
}