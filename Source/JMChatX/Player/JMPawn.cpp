#include "Player/JMPawn.h"

#include "JMChatX.h"


void AJMPawn::BeginPlay()
{
	Super::BeginPlay();

	FString NetRoleString = JMChatXFunctionLibrary::GetRoleString(this);
	
	FString CombinedString = FString::Printf(TEXT("CXPawn::BeginPlay() %s [%s]"), *JMChatXFunctionLibrary::GetNetModeString(this), *NetRoleString);
	
	JMChatXFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void AJMPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	Super::PossessedBy(NewController);
	
	FString NetRoleString = JMChatXFunctionLibrary::GetRoleString(this);
	
	FString CombinedString = FString::Printf(TEXT("CXPawn::PossessedBy() %s [%s]"), *JMChatXFunctionLibrary::GetNetModeString(this), *NetRoleString);

	JMChatXFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}
