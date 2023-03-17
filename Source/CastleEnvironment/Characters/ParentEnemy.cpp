// Fill out your copyright notice in the Description page of Project Settings.

#include "ParentEnemy.h"

#include "CastleEnvironment/MyGameStateBase.h"
#include "CastleEnvironment/UI/HealthBar.h"
#include "Components/WidgetComponent.h"

// Sets default values
AParentEnemy::AParentEnemy(){
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and attach the health bar
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthWidgetComponent->AttachToComponent( RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AParentEnemy::BeginPlay() {
	Super::BeginPlay();

	HealthBar = Cast<UHealthBar>(HealthWidgetComponent->GetUserWidgetObject());
	HealthBar->SetParentCharacterOwner(this);

	MyGameStateBase = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
}

// Called every frame
void AParentEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AParentEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AParentEnemy::UpdateHP() const {
	HealthBar->UpdateHP();
}

