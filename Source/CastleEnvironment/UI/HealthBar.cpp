// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"

#include "MovieScene.h"
#include "Animation/WidgetAnimation.h"
#include "CastleEnvironment/Characters/ParentEnemy.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Color.h"
#include "Containers/UnrealString.h"

void UHealthBar::NativeConstruct() {
	Super::NativeConstruct();
}

void UHealthBar::SetParentCharacterOwner(AParentEnemy* ParentCharacter) {
	ParentCharacterOwner = ParentCharacter;
}

void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UHealthBar::UpdateHP() const {
	if (ParentCharacterOwner.IsValid()) {
		const float HealthPercent = (ParentCharacterOwner->CurrentHealth / ParentCharacterOwner->MaxHealth);
		HealthProgressBar->SetPercent(HealthPercent);
		CurrentHealthLabel->SetText(FText::AsNumber(ParentCharacterOwner->CurrentHealth *100));
		MaxHealthLabel->SetText(FText::AsNumber(ParentCharacterOwner->MaxHealth *100));

		// TODO: Change this for a gradient rather than sudden changes, for a better visual effect
		FLinearColor HealthColour;
		if (HealthPercent <= .2) {
			// Red Health colour
			HealthColour = FLinearColor(1, 0.023671, 0, 1);
			HealthProgressBar->SetFillColorAndOpacity(HealthColour);
			
		}else if (HealthPercent > .2 && HealthPercent <= .5) {
			// Orange Health colour
			HealthColour = FLinearColor(1, .361054, .077669, 1);
			HealthProgressBar->SetFillColorAndOpacity(HealthColour);
		}else{
			// Green Health colour
			HealthColour = FLinearColor(.0, 1, .068355, 1);
			HealthProgressBar->SetFillColorAndOpacity(HealthColour);
		}
	}
}

void UHealthBar::FadeHealthBarAnimation() {
	// TODO: If HP bar is hidden, when this triggers it shows and plays the fade animation again. Disable if hidden not to run with a bool? Destroy the widget after hiding it?
    if (HealthFadeAnimation && !CurrentHealthLabel->GetText().ToString().Equals(TEXT("0"))) {
    	PlayAnimation(HealthFadeAnimation, 0 , 1, EUMGSequencePlayMode::Forward, 1);
    }
}


UWidgetAnimation* UHealthBar::GetAnimationByName(FName AnimationName) const {
	UWidgetAnimation* const* WidgetAnimation = AnimationsMap.Find(AnimationName);
	if (WidgetAnimation) {
		return *WidgetAnimation;
	}
	return nullptr;
}
