// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMyProjectile::AMyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!CollisionComponent)
	{
		// ��������м򵥵���ײչʾ��
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// ���������ײ�����ļ���������Ϊ"Projectile"��
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		// �������ĳ��ʱ���õ��¼���
		//CollisionComponent->OnComponentHit.AddDynamic(this, &AMyProjectile::OnHit);
		// �����������ײ�뾶��
		CollisionComponent->InitSphereRadius(50.0f);
		// �����������Ϊ��ײ�����
		RootComponent = CollisionComponent;

		//CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
		//CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		//CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		CollisionComponent->SetCollisionProfileName("Projectile");
	}

	if (!ParticleSystemComponent)
	{
		ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
		ParticleSystemComponent->SetupAttachment(RootComponent);
	}

	if (!ProjectileMovementComponent)
	{
		// ʹ�ô����������������ƶ����������Ĭ������
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);//�����˶��������
		
		ProjectileMovementComponent->InitialSpeed = 3000.0f;//��ʼ�ٶ�
		ProjectileMovementComponent->MaxSpeed = 3000.0f;//����ٶ�
		ProjectileMovementComponent->bRotationFollowsVelocity = true;//��ת�����ٶȷ���
		
		ProjectileMovementComponent->bShouldBounce = true;//���Ե���
		ProjectileMovementComponent->Bounciness = 0.3f;//����ϵ��
		
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;//����Ӱ������ϵ��

		bReplicates = true;
	}

	//������������þ�̬������
	//if (!ProjectileMeshComponent)
	//{
	//	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	//	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	//	if (Mesh.Succeeded())
	//	{
	//		ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
	//		ProjectileMeshComponent->SetRelativeScale3D(FVector(5.0f,5.0f,5.0f));
	//	}
	//}
	//������������ò���ʵ��
	//static ConstructorHelpers::FObjectFinder<UMaterial>MaterialForProjectile(TEXT("/Script/Engine.Material'/Engine/MapTemplates/Materials/BasicAsset03.BasicAsset03'"));
	//if (MaterialForProjectile.Succeeded())
	//{
	//	ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(MaterialForProjectile.Object, ProjectileMeshComponent);//�½�һ������ʵ�����ٽ���ʵ���Ĳ������ø������������
	//}
	//
	//ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	//ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	//ProjectileMeshComponent->SetupAttachment(RootComponent);
	
	
	// ������������������Ϊ5��
	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void AMyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);
			UE_LOG(LogTemp, Error, TEXT("Health minus 20!"));
			Destroy();
		}
	} 
}

// Called every frame
void AMyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ��ʼ����������Ϸ������ٶȵĺ�����
void AMyProjectile::FireInDirection(const FVector& ShootDirection) //�������͵ĳ�����ά������Ϊ�������
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;//����*�ٶȴ�С
}

void AMyProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CollisionComponent->OnComponentHit.AddDynamic(this, &AMyProjectile::OnHit);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectile::OnActorOverlap);
}

// ���������������ʱ����õĺ�����
void AMyProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}

	if (OtherActor)
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);
			UE_LOG(LogTemp, Warning, TEXT("Health minus 20!"));
		}
		if (GetInstigator() != OtherActor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Instigator is not same with attacked actor!"));
		}
	}

	Destroy();//��������
}