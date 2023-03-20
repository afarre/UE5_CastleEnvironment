// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthBar.h"
#include "GameFramework/HUD.h"
#include "PrimaryHUD.generated.h"

class UStaminaAndHealth;
class APirate;

/**
 * 
 */
UCLASS()
class CASTLEENVIRONMENT_API APrimaryHUD : public AHUD
{
	GENERATED_BODY()

public:
	APrimaryHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void UpdateHP(APirate* Pirate) const;

	UFUNCTION()
	void FadeHealthBarAnimation() const;
	void UpdateStamina(APirate* Pirate);

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> HealthAndStaminaWidget;

private:
	UStaminaAndHealth* StaminaAndHealth;
	
};
