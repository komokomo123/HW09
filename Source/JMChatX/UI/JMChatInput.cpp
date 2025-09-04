#include "UI/JMChatInput.h"

#include "Components/EditableTextBox.h"
#include "Player/JMPlayerController.h"

void UJMChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);		
	}	
}

void UJMChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UJMChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			AJMPlayerController* OwningCXPlayerController = Cast<AJMPlayerController>(OwningPlayerController);
			if (IsValid(OwningCXPlayerController) == true)
			{
				OwningCXPlayerController->SetChatMessageString(Text.ToString());
				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}
