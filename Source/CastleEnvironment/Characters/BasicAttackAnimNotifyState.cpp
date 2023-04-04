// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAttackAnimNotifyState.h"
#include "Engine.h"
#include "Pirate.h"
#include "SkeletonSwordsman.h"
#include "CastleEnvironment/Weapons/WarMace.h"


void UBasicAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) {
	if (MeshComp && MeshComp->GetOwner()) {
		if (const APirate* Pirate = Cast<APirate>(MeshComp->GetOwner()); Pirate && Pirate->WieldedWeapon) {
			UE_LOG(LogTemp, Warning, TEXT("\nNotifyBegin (animation)"));
			Pirate->WieldedWeapon->AttackStart(Pirate->WieldedWeapon->WeaponCollisionCapsule);
			Pirate->EffortGruntAudioComponent->Play(.0f);
			Pirate->AttackSlashAudioComponent->Play(.0f);
		}
	}
}

void UBasicAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) {
	if (MeshComp && MeshComp->GetOwner()) {
		if (APirate* Pirate = Cast<APirate>(MeshComp->GetOwner()); Pirate && Pirate->WieldedWeapon) {
			if (Pirate->WieldedWeapon->HasOverlapped) {
				if (Cast<ASkeletonSwordsman>(Pirate->WieldedWeapon->OverlappedActor)) {
					UE_LOG(LogTemp, Warning, TEXT("NotifyTick (animation) with HasOverlapped = true"));
					Pirate->AttackConnected(Pirate->WieldedWeapon->OverlappedActor);
				}	
			}
		}
	}
}

void UBasicAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	if (MeshComp && MeshComp->GetOwner()) {
		if (APirate* Pirate = Cast<APirate>(MeshComp->GetOwner()); Pirate && Pirate->WieldedWeapon) {
			UE_LOG(LogTemp, Warning, TEXT("NotifyEnd (animation)"));
			Pirate->WieldedWeapon->AttackEnd(Pirate->WieldedWeapon->WeaponCollisionCapsule);
			Pirate->ActOnConnectedAttacks();
		}
	}
}