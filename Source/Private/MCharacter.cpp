// Fill out your copyright notice in the Description page of Project Settings.


#include "MCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMCharacter::AMCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultCharacterSceneComp");//初始化根组件
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");//为指针SpringArmComp指定USpringArmComponent的实例，命名为SpringArmComp并且附在RootComponent下
	check(SpringArmComp != nullptr);
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");//为指针CameraComp指定UCameraComponent的实例，命名为CameraComp并且附在SpringArmComp下
	check(CameraComp != nullptr);
	CameraComp->SetupAttachment(SpringArmComp);
	//CameraComp->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));//此注释是单独生成CameraComp时并将其附在CapsuleComponent时的情况，用到了强制转换函数，但其实并非必要，可直接(GetCapsuleComponent()),因为CapusleComponent类型是USceneComponent的子类

	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));//由于后面bUsePawnControlRotation = true，所以其旋转角度设置为任意数值均不影响
	SpringArmComp->bUsePawnControlRotation = true;

	//CameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	//CameraComp->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AMCharacter::BeginPlay()
{
	Super::BeginPlay();//继承BeginPlay函数

	check(GEngine != nullptr);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter!"));//游戏开始时在屏幕上打印相关字符串，并设置其颜色为红色，停留5秒

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
}

//射击开火函数的定义
void AMCharacter::Fire()
{
	// 试图发射发射物。
	if (ProjectileClass)
	{
		// 获取摄像机的位置和旋转方向
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// 设置MuzzleOffset，在略靠近摄像机前生成发射物。
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// 将MuzzleOffset从摄像机空间变换到世界空间。
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// 使目标方向略向上倾斜。
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// 在枪口位置生成发射物。
			AMyProjectile* Projectile = World->SpawnActor<AMyProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// 设置发射物的初始轨迹。
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}