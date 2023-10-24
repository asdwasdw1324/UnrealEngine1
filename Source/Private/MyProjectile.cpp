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
		// 用球体进行简单的碰撞展示。
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// 将球体的碰撞配置文件名称设置为"Projectile"。
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		// 组件击中某物时调用的事件。
		//CollisionComponent->OnComponentHit.AddDynamic(this, &AMyProjectile::OnHit);
		// 设置球体的碰撞半径。
		CollisionComponent->InitSphereRadius(50.0f);
		// 将根组件设置为碰撞组件。
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
		// 使用此组件驱动发射物的移动。配置相关默认属性
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);//设置运动更新组件
		
		ProjectileMovementComponent->InitialSpeed = 3000.0f;//初始速度
		ProjectileMovementComponent->MaxSpeed = 3000.0f;//最大速度
		ProjectileMovementComponent->bRotationFollowsVelocity = true;//旋转跟随速度方向
		
		ProjectileMovementComponent->bShouldBounce = true;//可以弹跳
		ProjectileMovementComponent->Bounciness = 0.3f;//弹跳系数
		
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;//重力影响缩放系数

		bReplicates = true;
	}

	//发射物组件配置静态网格体
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
	//发射物组件配置材质实例
	//static ConstructorHelpers::FObjectFinder<UMaterial>MaterialForProjectile(TEXT("/Script/Engine.Material'/Engine/MapTemplates/Materials/BasicAsset03.BasicAsset03'"));
	//if (MaterialForProjectile.Succeeded())
	//{
	//	ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(MaterialForProjectile.Object, ProjectileMeshComponent);//新建一个材质实例，再将该实例的材质设置给所需的网格体
	//}
	//
	//ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	//ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	//ProjectileMeshComponent->SetupAttachment(RootComponent);
	
	
	// 该类产生后的生命周期为5秒
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

// 初始化射击方向上发射物速度的函数。
void AMyProjectile::FireInDirection(const FVector& ShootDirection) //引用类型的常量三维向量作为射击方向
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;//方向*速度大小
}

void AMyProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CollisionComponent->OnComponentHit.AddDynamic(this, &AMyProjectile::OnHit);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectile::OnActorOverlap);
}

// 当发射物击中物体时会调用的函数。
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

	Destroy();//立即销毁
}