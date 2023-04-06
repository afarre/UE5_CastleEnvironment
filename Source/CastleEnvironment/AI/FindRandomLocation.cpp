// Fill out your copyright notice in the Description page of Project Settings.


#include "FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AnimalAIController.h"
#include "Blackboard_Deer_Keys.h"

UFindRandomLocation::UFindRandomLocation(FObjectInitializer const& ObjectInitializer) {
	// Display node display name
	NodeName = TEXT("Find random location");
}

EBTNodeResult::Type UFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	// Get AI controller
	const AAnimalAIController* Controller = Cast<AAnimalAIController>(OwnerComp.GetAIOwner());
	APawn* Deer = Controller->GetPawn();

	// Get Deer's location to use as the origin location
	FVector const OriginLocation = Deer->GetActorLocation();

	// Get the navigation system and generate a random location on the NavMesh
	FNavLocation ResultLocation;
	UNavigationSystemV1* const NavigationSystemV1 = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavigationSystemV1->GetRandomPointInNavigableRadius(OriginLocation, SearchRadious, ResultLocation, nullptr)) {
		Controller->GetBlackBoard()->SetValueAsVector(BlackBoardKeys::TargetLocation, ResultLocation.Location);
	}

	// Finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
