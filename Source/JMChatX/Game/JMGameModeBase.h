#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JMGameModeBase.generated.h"

class AJMPlayerController;

/**
 * 
 */
UCLASS()
class JMCHATX_API AJMGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;

	void PrintChatMessageString(AJMPlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void IncreaseGuessCount(AJMPlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(AJMPlayerController* InChattingPlayerController, int InStrikeCount);

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<AJMPlayerController>> AllPlayerControllers;
};
