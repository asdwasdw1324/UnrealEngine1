// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "GameFramework\Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectileBase.h"
#include "MyProjectile.h"

class AAIController;
class ACharacter;

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());

		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector Muzzlocation = MyPawn->GetMesh()->GetSocketLocation("hand_r");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CharacterLocation"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector Direction = TargetActor->GetActorLocation() - Muzzlocation;
		FRotator MuzzleRotation = Direction.Rotation();
		FVector FinalMuzzLocation = Muzzlocation + MuzzleRotation.Vector() * 100;

		FActorSpawnParameters Params;
		Params.Owner = MyPawn;
		Params.Instigator = MyPawn;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FCollisionQueryParams AIQueryParams;
		AIQueryParams.AddIgnoredActor(MyPawn);

		FCollisionObjectQueryParams AIObjectParams;
		AIObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		AIObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
		AIObjectParams.AddObjectTypesToQuery(ECC_Pawn);

		FTransform AISpawnTM = FTransform(MuzzleRotation, FinalMuzzLocation);

		AMyProjectile* NewProj = nullptr;
		if (ensure(ProjectileClass))
		{
			ACharacter* TargetCharacter = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CharacterLocation"));
			check(AttackAnimation);
			TargetCharacter->PlayAnimMontage(AttackAnimation);

			NewProj = GetWorld()->SpawnActor<AMyProjectile>(ProjectileClass, AISpawnTM, Params);
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("NewProjName: %s"), *GetNameSafe(NewProj)));
		}
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Failed;
}
