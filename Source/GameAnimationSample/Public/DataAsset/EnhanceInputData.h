// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhanceInputData.generated.h"

class UInputMappingContext;
class UInputAction;
UCLASS()
class GAMEANIMATIONSAMPLE_API UEnhanceInputData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputMappingContext* InputMappingContext;

	//Dieu khien do sung
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_SwitchToPistol;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_SwitchToMelee;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_SwitchToRifle;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputAction* IA_ShootandAttack;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputAction* IA_Pickup;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputAction* IA_Reload;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputAction* IA_OpenInventoy;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputAction* IA_Aim;
};
