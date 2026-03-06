// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponent/ItemPickupComponent.h"
#include "AWorldItem.h"
#include "BaseCharacter.h"
// Sets default values for this component's properties
UItemPickupComponent::UItemPickupComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}



// Called when the game starts
void UItemPickupComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ABaseCharacter>(GetOwner());
	Inventory = Character->Inventory;
}


void UItemPickupComponent::PickupItem()
{	
	if (!HoveredItem) return;

	FName RowName = HoveredItem->ItemRowName.RowName;
	int32 CurrentQuantity = HoveredItem->ItemQuantity;
	Inventory->AddItemToSlotFromWorld(HoveredItem);
	if (HoveredItem)
	{
		HoveredItem->Destroy();
		HoveredItem = nullptr;
	}
	Inventory->LogAllItemSlots();
}
