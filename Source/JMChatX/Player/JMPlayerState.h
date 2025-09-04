#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "JMPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class JMCHATX_API AJMPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AJMPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;

	FString GetPlayerInfoString();
	
};
