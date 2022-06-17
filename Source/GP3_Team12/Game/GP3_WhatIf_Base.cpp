#include "GP3_WhatIf_Base.h"
#include "Kismet/GameplayStatics.h"

void UGP3_WhatIfBase::Setup_Implementation() { }
void UGP3_WhatIfBase::OnWhatIfActivated_Implementation() { }
bool UGP3_WhatIfBase::ShouldDeactivate_Implementation() { return true; }
UGP3_WhatIfBase* UGP3_WhatIfBase::OnWhatIfDeactivated_Implementation() { return nullptr; }
AGP3_CharacterBase* UGP3_WhatIfBase::GetPlayer()
{
	return Cast<AGP3_CharacterBase>(UGameplayStatics::GetPlayerControllerFromID(this, 0)->GetPawn());
}
