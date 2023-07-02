// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class CASTLEENVIRONMENT_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void Resume();

	void Settings();

	void Save();
	
	void Load();
	
	void Quit();

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
