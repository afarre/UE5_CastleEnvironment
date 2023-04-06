// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AAnimalAIController::AAnimalAIController(const FObjectInitializer& ObjectInitializer) {
	// Get the necessary references
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("/Script/AIModule.BehaviorTree'/Game/AnimalVarietyPack/BT_Deer.BT_Deer'"));
	if (obj.Succeeded()) {
		BehaviorTree = obj.Object;
	}
	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("Behaviour Component"));
	BlackboardComponent = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("Blackboard Component"));
}

void AAnimalAIController::BeginPlay() {
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComponent->StartTree(*BehaviorTree);
}

void AAnimalAIController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	if (BlackboardComponent) {
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}
}

UBlackboardComponent* AAnimalAIController::GetBlackBoard() const {
	return BlackboardComponent;
}
