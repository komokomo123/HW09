#include "Game/JMGameModeBase.h"

#include "EngineUtils.h"
#include "JMGameStateBase.h"
#include "Player/JMPlayerController.h"
#include "Player/JMPlayerState.h"

void AJMGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AJMPlayerController* JMPlayerController = Cast<AJMPlayerController>(NewPlayer);
	if (IsValid(JMPlayerController) == true)
	{
		JMPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		
		// 서버 GameMode가 관리하는 컨트롤러 리스트에 등록해요.
		AllPlayerControllers.Add(JMPlayerController);

		// 해당 컨트롤러의 PlayerState를 가져와 플레이어 이름을 부여해요. (예: "Player1", "Player2")
		AJMPlayerState* CXPS = JMPlayerController->GetPlayerState<AJMPlayerState>();
		if (IsValid(CXPS) == true)
		{
			CXPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		// GameState를 가져와서, 접속한 플레이어의 이름을 전체 클라이언트에게 알리는 메시지로 브로드캐스트해요.
		// - Multicast RPC: 서버 → 모든 클라이언트 전파
		AJMGameStateBase* JMGameStateBase =  GetGameState<AJMGameStateBase>();
		if (IsValid(JMGameStateBase) == true)
		{
			JMGameStateBase->MulticastRPCBroadcastLoginMessage(CXPS->PlayerNameString);
		}
	}
}

FString AJMGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());

	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool AJMGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;
		
	} while (false);	

	return bCanPlay;
}

FString AJMGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else 
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;				
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void AJMGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
}

void AJMGameModeBase::PrintChatMessageString(AJMPlayerController* InChattingPlayerController,
	const FString& InChatMessageString)
{
	FString ChatMessageString = InChatMessageString;

	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);

	if (IsGuessNumberString(GuessNumberString) == true)
	{
		AJMPlayerState* CXPS = InChattingPlayerController->GetPlayerState<AJMPlayerState>();
		if (IsValid(CXPS) == false)
		{
			return;
		}
		
		if (CXPS->CurrentGuessCount >= CXPS->MaxGuessCount)
		{
			InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("You have reached the maximum number of guesses."));
			return;
		}

		IncreaseGuessCount(InChattingPlayerController);

		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
		
		for (TActorIterator<AJMPlayerController> It(GetWorld()); It; ++It)
		{
			AJMPlayerController* JMPlayerController = *It;
			if (IsValid(JMPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				JMPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<AJMPlayerController> It(GetWorld()); It; ++It)
		{
			AJMPlayerController* CXPlayerController = *It;
			if (IsValid(CXPlayerController) == true)
			{
				CXPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void AJMGameModeBase::IncreaseGuessCount(AJMPlayerController* InChattingPlayerController)
{
	AJMPlayerState* CXPS = InChattingPlayerController->GetPlayerState<AJMPlayerState>();
	if (IsValid(CXPS) == true)
	{
		CXPS->CurrentGuessCount++;
	}
}

void AJMGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& CXPlayerController : AllPlayerControllers)
	{
		AJMPlayerState* CXPS = CXPlayerController->GetPlayerState<AJMPlayerState>();
		if (IsValid(CXPS) == true)
		{
			CXPS->CurrentGuessCount = 0;
		}
	}
}

void AJMGameModeBase::JudgeGame(AJMPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		AJMPlayerState* CXPS = InChattingPlayerController->GetPlayerState<AJMPlayerState>();
		for (const auto& CXPlayerController : AllPlayerControllers)
		{
			if (IsValid(CXPS) == true)
			{
				FString CombinedMessageString = CXPS->PlayerNameString + TEXT(" has won the game.");
				CXPlayerController->NotificationText = FText::FromString(CombinedMessageString);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& CXPlayerController : AllPlayerControllers)
		{
			AJMPlayerState* CXPS = CXPlayerController->GetPlayerState<AJMPlayerState>();
			if (IsValid(CXPS) == true)
			{
				if (CXPS->CurrentGuessCount < CXPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& CXPlayerController : AllPlayerControllers)
			{
				CXPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));

				ResetGame();
			}
		}
	}
}
