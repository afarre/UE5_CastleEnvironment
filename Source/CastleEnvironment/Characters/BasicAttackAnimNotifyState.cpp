// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAttackAnimNotifyState.h"
#include "Engine.h"
#include "Pirate.h"


void UBasicAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) {
	UE_LOG(LogTemp, Warning, TEXT("NotifyBegin"), );

	if (MeshComp && MeshComp->GetOwner()) {
		APirate* Pirate = Cast<APirate>(MeshComp->GetOwner());
		if (Pirate /*&& !Pirate->EffortGruntAudioComponent->IsPlaying() && !Pirate->AttackSlashAudioComponent->IsPlaying()*/) {
			Pirate->EffortGruntAudioComponent->Play(.0f);
			Pirate->AttackSlashAudioComponent->Play(.0f);
		}
	}
}

void UBasicAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) {
	UE_LOG(LogTemp, Warning, TEXT("NotifyTick"), );
}

void UBasicAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	UE_LOG(LogTemp, Warning, TEXT("NNotifyEnd"), );	
}