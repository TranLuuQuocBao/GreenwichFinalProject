// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InventorySlots.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Overlay.h"

void UInventorySlots::SetSlotData(UTexture2D* Icon, int32 Quantity, FText Description, bool bIsEmpty)
{
	if (bIsEmpty)
	{
		if (Overlay_01)
			Overlay_01->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	// Nếu có item → hiện overlay
	if (Overlay_01)
		Overlay_01->SetVisibility(ESlateVisibility::Visible);

	// Hiển thị icon nếu có
	if (ItemIcon)
	{
		if (Icon)
		{
			ItemIcon->SetBrushFromTexture(Icon);
			ItemIcon->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// Hiển thị số lượng nếu > 1
	if (itemQuantity)
	{
		if (Quantity > 1)
		{
			itemQuantity->SetText(FText::AsNumber(Quantity));
			itemQuantity->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			itemQuantity->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	SlotQuantity = Quantity;
	SlotDescription = Description;
}
void UInventorySlots::OnSlotClicked()
{
	UE_LOG(LogTemp, Log, TEXT("🟢 Clicked Item: %s | Quantity: %d"), *ItemRowName.ToString(), SlotQuantity);
}