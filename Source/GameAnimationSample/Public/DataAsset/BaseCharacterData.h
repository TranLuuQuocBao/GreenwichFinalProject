// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseCharacterData.generated.h"

class UMyUserWidget;
class UHealthWidget;
class UCrosshairWidget;
class AAWorldItem;
UCLASS()
class GAMEANIMATIONSAMPLE_API UBaseCharacterData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* PickupMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UDataTable* WeaponDatabase;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	UDataTable* ItemDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerData")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerData")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerInventorySlot")
	int32 MaxSlot;
	UPROPERTY(EditDefaultsOnly, Category = "InventoryWidget")
	class UInventorySlots* InventorySlotsWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMyUserWidget> InventoryBlueprintWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UInventorySlots> SlotWidgetBlueprintClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UHealthWidget> HealthWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCrosshairWidget> CrosshairWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryWidget")
	class UMyUserWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	TArray<TSubclassOf<AAWorldItem>> ItemsToSpawn;

};
