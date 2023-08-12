// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveActor.h"
#include "PhysicsEngine\RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogExplosiveActor, All, All)

// Sets default values
AExplosiveActor::AExplosiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	//�������滻��Ϊ��ը��ľ�̬������
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (Mesh.Succeeded())
	{
		MeshComp->SetStaticMesh(Mesh.Object);
	}

	//static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Water_Ocean.M_Water_Ocean'"));
	//if (Material.Succeeded())
	//{
	//	ExplosiveActorMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, MeshComp);
	//	MeshComp->SetMaterial(0, ExplosiveActorMaterialInstance);
	//}

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);
	ForceComp->SetAutoActivate(false);  

	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 200000.0f; // Alternative: 200000.0 if bImpulseVelChange = false
	// Optional, ignores 'Mass' of other objects (if false, the impulse strength will be much higher to push most objects depending on Mass)
	ForceComp->bImpulseVelChange = false;

	// Optional, default constructor of component already adds 4 object types to affect, excluding WorldDynamic
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);

	// Binding either in constructor or in PostInitializeComponents() below
	//MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

// Called when the game starts or when spawned
void AExplosiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveActor::PostInitializeComponents()
{
	// Don't forget to call parent function
	Super::PostInitializeComponents();

	MeshComp->OnComponentHit.AddDynamic(this, &AExplosiveActor::OnActorHit);
}

void AExplosiveActor::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	//UE_LOG�е�%sռλ����Ҫ��FString����ת��Ϊcont TCHAR*���Ͳ��ܱ�TEXT����ȷչ��
	//������UE_LOG����AddOnScreenDebugMessage�ж�����FString::Printf()����������ɸ�ʽ����FSring�������ٸ��ݾ����ʹ�ú���ȷ���Ƿ���Ҫ����
	UE_LOG(LogExplosiveActor, Warning, TEXT("OnActorHit in Explosive Ball"));
	UE_LOG(LogExplosiveActor, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 5.0f, true, 2.0f);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, CombinedString);
	UE_LOG(LogExplosiveActor, Display, TEXT("%s"), *CombinedString);
}