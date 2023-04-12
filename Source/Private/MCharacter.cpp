// Fill out your copyright notice in the Description page of Project Settings.


#include "MCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMCharacter::AMCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultCharacterSceneComp");//��ʼ�������
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");//Ϊָ��SpringArmCompָ��USpringArmComponent��ʵ��������ΪSpringArmComp���Ҹ���RootComponent��
	check(SpringArmComp != nullptr);
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");//Ϊָ��CameraCompָ��UCameraComponent��ʵ��������ΪCameraComp���Ҹ���SpringArmComp��
	check(CameraComp != nullptr);
	CameraComp->SetupAttachment(SpringArmComp);
	//CameraComp->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));//��ע���ǵ�������CameraCompʱ�����丽��CapsuleComponentʱ��������õ���ǿ��ת������������ʵ���Ǳ�Ҫ����ֱ��(GetCapsuleComponent()),��ΪCapusleComponent������USceneComponent������

	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));//���ں���bUsePawnControlRotation = true����������ת�Ƕ�����Ϊ������ֵ����Ӱ��
	SpringArmComp->bUsePawnControlRotation = true;

	//CameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	//CameraComp->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AMCharacter::BeginPlay()
{
	Super::BeginPlay();//�̳�BeginPlay����

	check(GEngine != nullptr);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter!"));//��Ϸ��ʼʱ����Ļ�ϴ�ӡ����ַ���������������ɫΪ��ɫ��ͣ��5��

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

//����������Ķ���
void AMCharacter::Fire()
{
	// ��ͼ���䷢���
	if (ProjectileClass)
	{
		// ��ȡ�������λ�ú���ת����
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// ����MuzzleOffset�����Կ��������ǰ���ɷ����
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// ��MuzzleOffset��������ռ�任������ռ䡣
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// ʹĿ�귽����������б��
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;
		//����ȷ���˷������λ�ú���ת����


		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// ��ǹ��λ�����ɷ����
			AMyProjectile* Projectile = World->SpawnActor<AMyProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// ���÷�����ĳ�ʼ�켣��
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}