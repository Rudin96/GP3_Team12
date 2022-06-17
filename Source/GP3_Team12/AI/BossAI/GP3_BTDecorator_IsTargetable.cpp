#include "GP3_BTDecorator_IsTargetable.h"
#include "GP3_Boss_AIController.h"
#include "GP3_AICharacterBase.h"
#include "GP3_Team12/Game/GP3_GameInstance.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#include "GP3_Team12/Character/GP3_CharacterBase.h"

UGP3_BTDecorator_IsTargetable::UGP3_BTDecorator_IsTargetable() {}

bool UGP3_BTDecorator_IsTargetable::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AActor* player = UGP3_GameInstance::GetPlayer(GetWorld());
	ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner());
	
	if (bossController != nullptr)
	{
		TArray<AActor*> allBossArea = bossController->GetAllBossArea();
		for (int i = 0; i < allBossArea.Num(); i++)
		{
			if (allBossArea[i]->IsOverlappingActor(player)) 
			{
				for (int k = 0; k < bossController->Children.Num(); k++)
				{
					auto currentChild = bossController->Children[k];
					if (currentChild.IsA<AGP3_AICharacterBase>() && allBossArea[i]->IsOverlappingActor(currentChild))
					{
						return true;
					}
				}
			}

		}
	}
	return false;
}

