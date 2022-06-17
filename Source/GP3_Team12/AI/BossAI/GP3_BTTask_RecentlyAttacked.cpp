#include "GP3_BTTask_RecentlyAttacked.h"
#include "AIController.h"
#include "GP3_AICharacterBase.h"

EBTNodeResult::Type UGP3_BTTaskNode_RecentlyAttacked::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto children = OwnerComp.GetAIOwner()->Children;
	AGP3_AICharacterBase* character = nullptr;
	for (int i = 0; i < children.Num(); i++)
	{
		if (children[i].IsA<AGP3_AICharacterBase>())
		{
			character = Cast<AGP3_AICharacterBase>(children[i]);
			if (RestTimer())
			{
				character->HasAttacked = false;
			}
			if (character->HasAttacked) 
			{
				return EBTNodeResult::Succeeded;
			}
			break;
		}
	}
	return EBTNodeResult::Failed;
}

bool UGP3_BTTaskNode_RecentlyAttacked::RestTimer()
{
	if (TimeStamp <= 0.f)
	{
		TimeStamp = RestTime;
		return true;
	}
	TimeStamp -= GetWorld()->GetDeltaSeconds();
	return false;
}
