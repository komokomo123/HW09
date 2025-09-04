#include "Game/JMGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/JMPlayerController.h"

void AJMGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			AJMPlayerController* CXPC = Cast<AJMPlayerController>(PC);
			if (IsValid(CXPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");

				CXPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
