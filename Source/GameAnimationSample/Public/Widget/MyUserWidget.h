// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/EWeaponState.h"
#include "Components/WrapBox.h"
#include "Widget/InventorySlots.h"
#include "CharacterComponent/InventorySystem.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEANIMATIONSAMPLE_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void CreateItemSlots();
	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryWrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UInventorySlots> InventorySlotWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	UInventorySystem* InventoryComponent;



	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	UDataTable* ItemDataTable;;

};
