// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaminaAndHealth.generated.h"

class APirate;
class UProgressBar;
class UTextBlock;
class UWidgetAnimation;
/**
 * 
 */
UCLASS()
class CASTLEENVIRONMENT_API UStaminaAndHealth : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void FadeHealthBarAnimation();
	void SetPirateParent(APirate* Pirate);
	void UpdatePirateHP(const APirate* Pirate) const;
	void UpdatePirateStamina(APirate* Pirate);

private:

	TWeakObjectPtr<APirate> ParentPirate;
	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* StaminaProgressBar;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CurrentHealthLabel;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* MaxHealthLabel;
	
	TMap<FName, UWidgetAnimation*> AnimationsMap;
	UWidgetAnimation* GetAnimationByName(FName AnimationName) const;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* HealthFadeAnimation;
};
