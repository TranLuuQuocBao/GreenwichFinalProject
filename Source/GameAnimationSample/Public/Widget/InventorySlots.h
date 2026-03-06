// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/EWeaponState.h"
#include "InventorySlots.generated.h"

/**
 * 
 */
UCLASS()
class GAMEANIMATIONSAMPLE_API UInventorySlots : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemIcon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* itemQuantity;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_30;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Overlay_01;

	UPROPERTY(BlueprintReadOnly)
	FName ItemRowName;

	UPROPERTY(BlueprintReadOnly)
	FText SlotDescription;

	UPROPERTY(BlueprintReadOnly)
	int32 SlotQuantity;

	UFUNCTION(BlueprintCallable)
	void SetSlotData(UTexture2D* Icon, int32 Quantity, FText Description, bool bIsEmpty);

	void OnSlotClicked();	
};
