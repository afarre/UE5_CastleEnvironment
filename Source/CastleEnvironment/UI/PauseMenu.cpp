// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"

#include "CastleEnvironment/Characters/Pirate.h"
#include "Kismet/GameplayStatics.h"


void UPauseMenu::Resume() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Resume"));
	APlayerController* PlayerController = Cast<APlayerController>(Pirate->GetController());
	// Disable cursor
	PlayerController->bShowMouseCursor = false; 
	PlayerController->bEnableClickEvents = false; 
	PlayerController->bEnableMouseOverEvents = false;

	// Unpause and resume game
	UGameplayStatics::SetGamePaused(GetWorld(),false);
	this->RemoveFromParent();
	Pirate->IsPaused = false;
}

void UPauseMenu::Settings() {}

void UPauseMenu::Save() {}

void UPauseMenu::Load() {}

void UPauseMenu::Quit() {
	// A bit of a redundant function as BP already have the Quit Game method, but whatever
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}
