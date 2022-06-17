#include "GP3_BTTask_Attack.h"

#include "AIController.h"
#include "GP3_Team12/Enemy/GP3_Boss_AttackBase.h"
#include "GP3_AICharacterBase.h"
#include <GP3_Team12/Enemy/GP3_Boss_ProjectileLaunchAttack.h>

UBTTask_Attack::UBTTask_Attack() {}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UActorComponent* attackClass = OwnerComp.GetAIOwner()->GetPawn()->GetComponentByClass(AttackClass);
	UGP3_BossAttackBase* attackBase = Cast<UGP3_BossAttackBase>(attackClass);
	attackBase->Attack();

	auto children = OwnerComp.GetAIOwner()->Children;
	AGP3_AICharacterBase* character = nullptr;
	for (int i = 0; i < children.Num(); i++)
	{
		if (children[i].IsA<AGP3_AICharacterBase>())
		{
			character = Cast<AGP3_AICharacterBase>(children[i]);
			break;
		}
	}
	return EBTNodeResult::Succeeded;
}