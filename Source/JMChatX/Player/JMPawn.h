#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "JMPawn.generated.h"

UCLASS()
class JMCHATX_API AJMPawn : public APawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	virtual void PossessedBy(AController* NewController) override;

};

