// Fill out your copyright notice in the Description page of Project Settings.

#include "ParentWeapon.h"
#include "CastleEnvironment/MyGameStateBase.h"

// Sets default values
AParentWeapon::AParentWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AParentWeapon::BeginPlay()
{
	Super::BeginPlay();

	MyGameStateBase = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
	
}


// Called every frame
void AParentWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}