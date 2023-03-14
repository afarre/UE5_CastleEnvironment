// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class AParentCharacter;
class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class CASTLEENVIRONMENT_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetParentCharacterOwner(AParentCharacter* ParentCharacter);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	TWeakObjectPtr<AParentCharacter> ParentCharacterOwner;
	
	// meta=(BindWidget) prompts a warning on the editor when compiling if this required Widget is missing
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CurrentHealthLabel;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* MaxHealthLabel;
	
	
};
