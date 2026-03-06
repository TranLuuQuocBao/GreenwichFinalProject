// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Data/EWeaponState.h"

#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMEANIMATIONSAMPLE_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	EWeaponSlot CurrentWeaponSlot;
	void NativeUpdateAnimation(float DeltaSeconds);

	UPROPERTY(BlueprintReadWrite, Category = "IK")
	FTransform LeftHandIKTransform;
};
