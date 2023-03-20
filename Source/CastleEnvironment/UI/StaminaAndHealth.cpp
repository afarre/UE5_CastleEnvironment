// Fill out your copyright notice in the Description page of Project Settings.


#include "CastleEnvironment/UI/StaminaAndHealth.h"

#include "MovieScene.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Color.h"
#include "CastleEnvironment/Characters/Pirate.h"

void UStaminaAndHealth::NativeConstruct() {
	Super::NativeConstruct();
}

void UStaminaAndHealth::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);
}


void UStaminaAndHealth::FadeHealthBarAnimation() {
	UE_LOG(LogTemp, Warning, TEXT("FadeHealthAnimation"),);
    if (HealthFadeAnimation) {
    	UE_LOG(LogTemp, Warning, TEXT("HealthFadeAnimation exists"),);
    	PlayAnimation(HealthFadeAnimation, 0 , 1, EUMGSequencePlayMode::Forward, 1);
    }
}

void UStaminaAndHealth::SetPirateParent(APirate* Pirate) {
	ParentPirate = Pirate;
}

void UStaminaAndHealth::UpdatePirateHP(const APirate* Pirate) const {
	const float HealthPercent = (Pirate->CurrentHealth / Pirate->MaxHealth);
	HealthProgressBar->SetPercent(HealthPercent);
	CurrentHealthLabel->SetText(FText::AsNumber(Pirate->CurrentHealth *100));
	MaxHealthLabel->SetText(FText::AsNumber(Pirate->MaxHealth *100));

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
		//HealthColour = FLinearColor(.0, 1, .068355, 1);
		//HealthProgressBar->SetFillColorAndOpacity(HealthColour);
	}
}

UWidgetAnimation* UStaminaAndHealth::GetAnimationByName(FName AnimationName) const {
	UWidgetAnimation* const* WidgetAnimation = AnimationsMap.Find(AnimationName);
	if (WidgetAnimation) {
		return *WidgetAnimation;
	}
	return nullptr;
}
