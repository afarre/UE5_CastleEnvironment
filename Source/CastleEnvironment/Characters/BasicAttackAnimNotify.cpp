// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAttackAnimNotify.h"

#include "Engine.h"
#include "Pirate.h"


void UBasicAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
/*
	UE_LOG(LogTemp, Warning, TEXT("Notify basic attack"), );
	if (MeshComp && MeshComp->GetOwner()) {
		APirate* Pirate = Cast<APirate>(MeshComp->GetOwner());
		if (Pirate && !Pirate->EffortGruntAudioComponent->IsPlaying()) {
			UE_LOG(LogTemp, Warning, TEXT("WTF PLAY"), );
			Pirate->EffortGruntAudioComponent->Play(.0f);
		}
	}
 */
}
