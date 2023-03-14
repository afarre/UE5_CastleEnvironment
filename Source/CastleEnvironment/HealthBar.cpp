// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"

#include "ParentCharacter.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Math/Color.h"

void UHealthBar::SetParentCharacterOwner(AParentCharacter* ParentCharacter) {
	ParentCharacterOwner = ParentCharacter;
}

void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ParentCharacterOwner.IsValid()) {
		const float HealthPercent = (ParentCharacterOwner->CurrentHealth / ParentCharacterOwner->MaxHealth);
		HealthBar->SetPercent(HealthPercent);
		CurrentHealthLabel->SetText(FText::AsNumber(ParentCharacterOwner->CurrentHealth *100));
		MaxHealthLabel->SetText(FText::AsNumber(ParentCharacterOwner->MaxHealth *100));

		// TODO: Change this for a gradient rather than sudden changes, for a better visual effect
		FLinearColor HealthColour;
		if (HealthPercent <= .2) {
			// Red Health colour
			HealthColour = FLinearColor(1, 0.023671, 0, 1);
			HealthBar->SetFillColorAndOpacity(HealthColour);
		}else if (HealthPercent > .2 && HealthPercent <= .5) {
			// Orange Health colour
			HealthColour = FLinearColor(1, .361054, .077669, 1);
			HealthBar->SetFillColorAndOpacity(HealthColour);
		}else{
			// Green Health colour
			HealthColour = FLinearColor(.0, 1, .068355, 1);
			HealthBar->SetFillColorAndOpacity(HealthColour);
		}
	}
}
