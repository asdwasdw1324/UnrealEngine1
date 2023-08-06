// Fill out your copyright notice in the Description page of Project Settings.


#include "MCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "SInteractionComponent.h"
#include "MyProjectile.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogMCharacter, All, All)

// Sets default values
AMCharacter::AMCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//��ʼ�������,���Character������,��������
	//RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultCharacterSceneComp");
	
	//Ϊָ��SpringArmCompָ��USpringArmComponent��ʵ��������ΪSpringArmComp���Ҹ���RootComponent��
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	check(SpringArmComp != nullptr);
	SpringArmComp->SetupAttachment(RootComponent);
	//SpringArmĬ�ϲ�������
	SpringArmComp->TargetArmLength = 250.f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 4.0f;
	SpringArmComp->SocketOffset = FVector(0.0f, 90.0f, 0.0f);

	//Ϊָ��CameraCompָ��UCameraComponent��ʵ��������ΪCameraComp���Ҹ���SpringArmComp��
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
	//�̳�BeginPlay����
	Super::BeginPlay();

	UE_LOG(LogMCharacter, Display, TEXT("Character born!/nGame Start!"));
}

//4���ƶ����������Ķ���
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

void AMCharacter::ToggleFreeCameraModeFree() 
{
	bFreeCameraMode = true;
}

void AMCharacter::ToggleFreeCameraModeLock() 
{
	bFreeCameraMode = false;
}

void AMCharacter::Fire_TimeElapsed()
{
	// ��ͼ���䷢���
	if (ProjectileClass1)
	{

		// ��ȡ�������λ�ú���ת���򣬽�actoreyesviewpoint��location��rotation��ֵ�ֱ𷵻ظ���������
		//FVector CameraLocation;
		//FRotator CameraRotation;
		//GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// ����MuzzleOffset�����Կ��������ǰ���ɷ������������һ�����λ������
		//MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// ��MuzzleOffset��������ռ�任������ռ䡣
		//FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// ʹĿ�귽����������б��
		//FRotator MuzzleRotation = CameraRotation;
		//MuzzleRotation.Pitch += 10.0f;
		//����ȷ���˷������λ�ú���ת������MuzzleLocation��MuzzleRotationΪ�������

		FVector MuzzleLocation = GetMesh()->GetSocketLocation("hand_r");
		FRotator MuzzleRotation = Controller->GetControlRotation();
		FTransform SpawnTM = FTransform(MuzzleRotation, MuzzleLocation);
		
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AMyProjectile* Projectile = nullptr;
			// ��ǹ��λ�����ɷ�����,������ΪProjectile��ָ��AMyProjectile��ָ�����
			if (ProjectileClass1)
			{
				Projectile = World->SpawnActor<AMyProjectile>(ProjectileClass1, SpawnTM, SpawnParams);
			}

			if (Projectile)
			{
				// �����ɷ�����֮�����÷�����ĳ�ʼ�켣������FireInDirection��������Ͷ�����ƶ������ʼʸ���ٶ�
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);

				//�����ɺ������ɫ����ǰһ���������
				MuzzleRotation.Pitch = 0;
				MuzzleRotation.Roll = 0;
				SetActorRotation(MuzzleRotation);
			}
		}
	}
	else
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Nothing to Fire!"));
	}
}

// Called every frame
void AMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString RotationString = Controller->GetControlRotation().ToString();
	GEngine->AddOnScreenDebugMessage(-1, .0f, FColor::Cyan, FString::Printf(TEXT("CurrentControllerRotation: %s"), *RotationString));
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
	//PlayerInputComponent->BindAction("FreeCamera", IE_Pressed, this, &AMCharacter::ToggleFreeCameraModeFree);
	//PlayerInputComponent->BindAction("FreeCamera", IE_Released, this, &AMCharacter::ToggleFreeCameraModeLock);
}

void AMCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

//����������Ķ���
void AMCharacter::Fire()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AMCharacter::Fire_TimeElapsed, 0.2f);
		
}