// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AnimalAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class CASTLEENVIRONMENT_API AAnimalAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAnimalAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	
	UBlackboardComponent* GetBlackBoard() const;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=AI, meta=(AllowPrivateAccess = "true"))
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=AI, meta=(AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UBlackboardComponent* BlackboardComponent;
};
