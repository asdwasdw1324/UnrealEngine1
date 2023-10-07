// Fill out your copyright notice in the Description page of Project Settings.


#include "MCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "SInteractionComponent.h"
#include "MyProjectile.h"
#include "Components/CapsuleComponent.h"
#include "SAttributeComponent.h"
#include "ProjectileBase.h"
#include "DashProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

//Define a customize log message for current CPP file
DEFINE_LOG_CATEGORY_STATIC(LogMCharacter, All, All)

// Sets default values
AMCharacter::AMCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//If true, this Pawn's yaw will be updated to match the Controller's ControlRotation yaw, if controlled by a PlayerController.
	//false = freedom view
	bUseControllerRotationYaw = false;

	//无需初始化根组件,针对Character类会出错
	//RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultCharacterSceneComp");
	
	//为指针SpringArmComp指定USpringArmComponent的实例，命名为SpringArmComp并且附在RootComponent下
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	check(SpringArmComp != nullptr);
	SpringArmComp->SetupAttachment(RootComponent);
	//SpringArm默认参数设置
	SpringArmComp->TargetArmLength = 250.f;
	SpringArmComp->bUsePawnControlRotation = true; //Removed from USpringArmComponent since UE5.2
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 4.0f;
	SpringArmComp->SocketOffset = FVector(0.0f, 90.0f, 0.0f);

	//为指针CameraComp指定UCameraComponent的实例，命名为CameraComp并且附在SpringArmComp下
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	check(CameraComp != nullptr);
	CameraComp->SetupAttachment(SpringArmComp);
	
	//获取角色移动组件并修改相关属性
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//角色属性组件，包含健康值等
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
}

// Called when the game starts or when spawned
void AMCharacter::BeginPlay()
{
	//继承BeginPlay函数
	Super::BeginPlay();

	UE_LOG(LogMCharacter, Error, TEXT("Character born!\nGame Start!"));
}

//4个移动操作函数的定义
void AMCharacter::MoveForward(float value) 
{
	//AddMovementInput(GetActorForwardVector(), value);
	//Get controller's forward vector
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	if (value != 0)
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("CurrentMoveDirection: %s"), *(Direction.ToString())));
		AddMovementInput(Direction, value);
	}
}

void AMCharacter::MoveRight(float value) 
{
	//AddMovementInput(GetActorRightVector(), value);
	//Get controller's right vector
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	if (value !=0)
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("CurrentMoveDirection: %s"), *(Direction.ToString())));
		AddMovementInput(Direction, value);
	}
}

//Not finished
void AMCharacter::ToggleFreeCameraModeFree() 
{
	bFreeCameraMode = true;
}

//Not finished
void AMCharacter::ToggleFreeCameraModeLock() 
{
	bFreeCameraMode = false;
}

void AMCharacter::Fire_TimeElapsed()
{
	// 试图发射发射物。
	if (ProjectileClass1 || ProjectileClass2)
	{
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

		FVector MuzzleLocation = GetMesh()->GetSocketLocation("hand_r");
		FRotator MuzzleRotation = Controller->GetControlRotation();
		
		if (GetWorld())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FCollisionShape Shape;
			Shape.SetSphere(20.0f);

			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);

			FCollisionObjectQueryParams ObjParams;
			ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
			ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
			ObjParams.AddObjectTypesToQuery(ECC_Pawn);

			FVector TraceStart = CameraComp->GetComponentLocation();
			FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000.0f);

			FHitResult Hit;

			if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
			{
				TraceEnd = Hit.ImpactPoint;
			}

			FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - MuzzleLocation).Rotator();
			FTransform SpawnTM = FTransform(ProjRotation, MuzzleLocation);

			AMyProjectile* Projectile1 = nullptr;
			AProjectileBase* Projectile2 = nullptr;
			// 在枪口位置生成发射物,定义名为Projectile的指向AMyProjectile的指针变量
			if (ProjectileClass1)
			{
				Projectile1 = GetWorld()->SpawnActor<AMyProjectile>(ProjectileClass1, SpawnTM, SpawnParams);
			}
			
			if (ProjectileClass2)
			{
				Projectile2 = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass2, SpawnTM, SpawnParams);
			}

			if (Projectile1 || Projectile2)
			{
				// 在生成发射物之后设置发射物的初始轨迹，调用FireInDirection函数给予投掷物移动组件初始矢量速度
				if (Projectile1)
				{
					FVector LaunchDirection = ProjRotation.Vector();
					Projectile1->FireInDirection(LaunchDirection);
				}

				if (Projectile2)
				{
					FVector LaunchDirection = ProjRotation.Vector();
					UProjectileMovementComponent* MovementComponent = Projectile2->FindComponentByClass<UProjectileMovementComponent>();
					MovementComponent->Velocity = LaunchDirection * MovementComponent->InitialSpeed;
				}
				

				//射击完成后调整角色朝向前一次射击方向
				ProjRotation.Pitch = 0;
				ProjRotation.Roll = 0;
				SetActorRotation(ProjRotation);
			}
		}
	}
	else
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Projectile is empty!"));
	}

	/*if (ProjectileClass2)
	{
		FVector MuzzleLocation = GetMesh()->GetSocketLocation("hand_r");
		FRotator MuzzleRotation = Controller->GetControlRotation();

		if (GetWorld())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FCollisionShape Shape;
			Shape.SetSphere(20.0f);

			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);

			FCollisionObjectQueryParams ObjParams;
			ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
			ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
			ObjParams.AddObjectTypesToQuery(ECC_Pawn);

			FVector TraceStart = CameraComp->GetComponentLocation();
			FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000.0f);

			FHitResult Hit;

			if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
			{
				TraceEnd = Hit.ImpactPoint;
			}

			FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - MuzzleLocation).Rotator();
			FTransform SpawnTM = FTransform(ProjRotation, MuzzleLocation);

			AProjectileBase* Projectile = nullptr;
			if (ProjectileClass2)
			{
				Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass2, SpawnTM, SpawnParams);
			}

			if (Projectile)
			{
				FVector LaunchDirection = ProjRotation.Vector();
				UProjectileMovementComponent* MovementComponent = Projectile->FindComponentByClass<UProjectileMovementComponent>();
				MovementComponent->Velocity = LaunchDirection * MovementComponent->InitialSpeed;

				ProjRotation.Pitch = 0;
				ProjRotation.Roll = 0;
				SetActorRotation(ProjRotation);
			}
		}
	}
	else
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("ProjectileClass2 is empty!"));
	}*/
}

// Called every frame
void AMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//以下三种方法等价
	//FString MyString = TEXT("Hello, world!");
	//FString MyString = FString(TEXT("Hello, world!"));
	//FString MyString = "Hello, world!";

	//FString::Printf常用于创建格式化的FString对象
	//FString MyVariable = "world";
	//FString FormattedString = FString::Printf(TEXT("Hello, %s!"), *MyVariable);

	FString RotationString = Controller->GetControlRotation().ToString();
	GEngine->AddOnScreenDebugMessage(-1, .0f, FColor::Cyan, FString::Printf(TEXT("CurrentControllerRotation: %s"), *RotationString));
	GEngine->AddOnScreenDebugMessage(-1, .0f, FColor::Cyan, FString::Printf(TEXT("Current Health: %f"), AttributeComp->Health));
}

// Called to bind functionality to input
void AMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMCharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMCharacter::Fire);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AMCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("DashFire", IE_Pressed, this, &AMCharacter::DashFire);
	PlayerInputComponent->BindAction("FreeCamera", IE_Pressed, this, &AMCharacter::ToggleFreeCameraModeFree);
	PlayerInputComponent->BindAction("FreeCamera", IE_Released, this, &AMCharacter::ToggleFreeCameraModeLock);
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
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AMCharacter::Fire_TimeElapsed, 0.2f);
		
}

void AMCharacter::DashFire()
{
	if (ProjectileClass3)
	{
		FRotator MuzzleRotation = Controller->GetControlRotation();
		FVector MuzzleLocation = GetActorLocation() + MuzzleRotation.Vector() * 100;
		FTransform SpawnTM = FTransform(MuzzleRotation, MuzzleLocation);

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ADashProjectile* DashProjectile = nullptr;
			DashProjectile = World->SpawnActor<ADashProjectile>(ProjectileClass3, SpawnTM, SpawnParams);

			MuzzleRotation.Pitch = 0;
			MuzzleRotation.Roll = 0;
			SetActorRotation(MuzzleRotation);
			
		}
	}
	else
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Nothing to DashFire!"));
	}
}
