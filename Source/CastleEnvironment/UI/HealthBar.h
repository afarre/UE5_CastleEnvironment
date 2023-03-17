// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CastleEnvironment/Characters/Pirate.h"
#include "HealthBar.generated.h"

class AParentEnemy;
class UProgressBar;
class UTextBlock;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class CASTLEENVIRONMENT_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Inline function: https://learn.microsoft.com/en-us/cpp/cpp/inline-functions-cpp?view=msvc-170
	void SetParentCharacterOwner(AParentEnemy* ParentCharacter)/*{ParentCharacterOwner = ParentCharacter;}*/;
	
	void UpdateHP() const;

	void FadeHealthBarAnimation();
	void SetPirateParent(APirate* Pirate);
	void UpdatePirateHP(const APirate* Pirate) const;

private:

	TWeakObjectPtr<AParentEnemy> ParentCharacterOwner;
	TWeakObjectPtr<APirate> ParentPirate;
	
	// meta=(BindWidget) prompts a warning on the editor when compiling if this required Widget is missing
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthProgressBar;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CurrentHealthLabel;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* MaxHealthLabel;
	
	//	void StoreWidgetAnimations();
	
	TMap<FName, UWidgetAnimation*> AnimationsMap;
	UWidgetAnimation* GetAnimationByName(FName AnimationName) const;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* HealthFadeAnimation;
	
};
