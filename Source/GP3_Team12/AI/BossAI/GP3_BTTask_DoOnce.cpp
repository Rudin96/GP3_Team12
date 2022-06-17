// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3_BTTask_DoOnce.h"

EBTNodeResult::Type UGP3_BTTask_DoOne::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!ConsiderItDone)
	{
		ConsiderItDone = true;
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
