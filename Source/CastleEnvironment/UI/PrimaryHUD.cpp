// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryHUD.h"

#include "CastleEnvironment/Characters/Pirate.h"

APrimaryHUD::APrimaryHUD() {
	
}

void APrimaryHUD::BeginPlay() {
	Super::BeginPlay();

	if (HealthBarWidgetClass) {
		HealthBarWidget = CreateWidget<UHealthBar>(GetWorld(), HealthBarWidgetClass);

		if (HealthBarWidget) {
			HealthBarWidget->AddToViewport();
		}
	}
}

void APrimaryHUD::UpdateHP(APirate* Pirate) const {
	HealthBarWidget->UpdatePirateHP(Pirate);
}

void APrimaryHUD::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void APrimaryHUD::DrawHUD() {
	Super::DrawHUD();
}

void APrimaryHUD::FadeHealthBarAnimation() const {
	 HealthBarWidget->FadeHealthBarAnimation();
}

