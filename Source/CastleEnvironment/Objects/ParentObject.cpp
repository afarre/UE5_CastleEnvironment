// Fill out your copyright notice in the Description page of Project Settings.

#include "ParentObject.h"
#include "CastleEnvironment/MyGameStateBase.h"

// Sets default values
AParentObject::AParentObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AParentObject::BeginPlay()
{
	Super::BeginPlay();

	MyGameStateBase = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
}

// Called every frame
void AParentObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

