#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "JMGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class JMCHATX_API AJMGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));
};
