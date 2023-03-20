// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryHUD.h"
#include "CastleEnvironment/UI/StaminaAndHealth.h"
#include "CastleEnvironment/Characters/Pirate.h"

APrimaryHUD::APrimaryHUD() {
	
}

void APrimaryHUD::BeginPlay() {
	Super::BeginPlay();

	if (HealthAndStaminaWidget) {
		StaminaAndHealth = CreateWidget<UStaminaAndHealth>(GetWorld(), HealthAndStaminaWidget);

		if (StaminaAndHealth) {
			StaminaAndHealth->AddToViewport();
		}
	}
}

void APrimaryHUD::UpdateHP(APirate* Pirate) const {
	StaminaAndHealth->UpdatePirateHP(Pirate);
}

void APrimaryHUD::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void APrimaryHUD::DrawHUD() {
	Super::DrawHUD();
}

void APrimaryHUD::FadeHealthBarAnimation() const {
	 StaminaAndHealth->FadeHealthBarAnimation();
}

void APrimaryHUD::UpdateStamina(APirate* Pirate) {
	//MaxHealthLabel;
}

