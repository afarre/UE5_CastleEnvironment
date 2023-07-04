// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

class APirate;
/**
 * 
 */
UCLASS()
class CASTLEENVIRONMENT_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Resume();

	UFUNCTION(BlueprintCallable)
	void Settings();

	UFUNCTION(BlueprintCallable)
	void Save();

	UFUNCTION(BlueprintCallable)
	void Load();

	UFUNCTION(BlueprintCallable)
	void Quit();
	
	APirate* Pirate;
	
private:
	UPROPERTY(meta=(BindWidget))
    UButton* ResumeButton;
    
    UPROPERTY(meta=(BindWidget))
    UButton* SettingsButton;
    
    UPROPERTY(meta=(BindWidget))
    UButton* SaveButton;
    
    UPROPERTY(meta=(BindWidget))
    UButton* LoadButton;

    UPROPERTY(meta=(BindWidget))
    UButton* QuitButton;
};
