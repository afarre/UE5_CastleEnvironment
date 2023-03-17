// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthBar.h"
#include "GameFramework/HUD.h"
#include "PrimaryHUD.generated.h"

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
	
	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

private:
	UHealthBar* HealthBarWidget;
	
	// meta=(BindWidget) prompts a warning on the editor when compiling if this required Widget is missing
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthProgressBar;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CurrentHealthLabel;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* MaxHealthLabel;
	
};
