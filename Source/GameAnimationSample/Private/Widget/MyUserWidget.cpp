// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MyUserWidget.h"

void UMyUserWidget::CreateItemSlots()
{
	if (InventoryWrapBox)
	InventoryWrapBox->ClearChildren();
	const TArray<FItemSlot>& ItemSlots = InventoryComponent->ItemSlot;
	const int32 TotalSlots = InventoryComponent->MaxSlot;

	for (int32 i = 0; i < TotalSlots; ++i)
	{
		UInventorySlots* SlotWidget = CreateWidget<UInventorySlots>(this, InventorySlotWidgetClass);
		if (!SlotWidget)
			continue;

		if (ItemSlots.IsValidIndex(i))
		{
			const FItemSlot& CurrentSlot = ItemSlots[i];
			if (!CurrentSlot.IsEmpty && ItemDataTable)
			{
				const FInventoryItemData* ItemData = ItemDataTable->FindRow<FInventoryItemData>(
					CurrentSlot.ItemRowName, TEXT("FromUI"));

				if (ItemData)
				{
					SlotWidget->SetSlotData(ItemData->Icon, CurrentSlot.SlotQuantity, ItemData->Description, false);
				}
				else
				{
					SlotWidget->SetSlotData(nullptr, 0, FText::GetEmpty(), true);
				}
			}
			else
			{
				SlotWidget->SetSlotData(nullptr, 0, FText::GetEmpty(), true);
			}
		}

		InventoryWrapBox->AddChildToWrapBox(SlotWidget);
	}
}
