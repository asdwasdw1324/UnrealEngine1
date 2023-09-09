// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "GameFramework\Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectileBase.h"

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

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector Direction = TargetActor->GetActorLocation() - Muzzlocation;
		FRotator MuzzleRotation = Direction.Rotation();
		FVector FinalMuzzLocation = Muzzlocation + MuzzleRotation.Vector() * 100;

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AProjectileBase* NewProj = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, FinalMuzzLocation, MuzzleRotation, Params);

		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Failed;
}
