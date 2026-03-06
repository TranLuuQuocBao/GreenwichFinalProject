// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/BaseAnimInstance.h"
#include "BaseCharacter.h"
void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* OwningPawn = TryGetPawnOwner();
	if (!OwningPawn) return;

	class ABaseCharacter* Character = Cast<ABaseCharacter>(OwningPawn);
	if (!Character) return;
	LeftHandIKTransform = Character->GetHandGunTransform();
	CurrentWeaponSlot = Character->WeaponSlot;
}


