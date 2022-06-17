#include "GP3_BTTask_SetHasAttacked.h"
#include "AIController.h"
#include "GP3_AICharacterBase.h"

EBTNodeResult::Type UGP3_BTTask_SetHasAttacked::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto children = OwnerComp.GetAIOwner()->Children;
	AGP3_AICharacterBase* character = nullptr;
	for (int i = 0; i < children.Num(); i++)
	{
		if (children[i].IsA<AGP3_AICharacterBase>())
		{
			character = Cast<AGP3_AICharacterBase>(children[i]);			
			character->HasAttacked = true;
		}
	}
	return EBTNodeResult::Succeeded;
}
