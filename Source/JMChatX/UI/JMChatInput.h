#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JMChatInput.generated.h"

class UEditableTextBox;

/**
 * 
 */
UCLASS()
class JMCHATX_API UJMChatInput : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;

	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
