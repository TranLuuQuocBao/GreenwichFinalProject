// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponent/InventorySystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon/BaseWeapon.h"
#include "BaseCharacter.h"
#include "DataAsset/BaseCharacterData.h"
// Sets default values for this component's properties
UInventorySystem::UInventorySystem()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UInventorySystem::SetupInventoryComponent(UBaseCharacterData* BCD)
{
	BaseCharacterData = BCD;
}

void UInventorySystem::BeginPlay()
{
	Super::BeginPlay();

	InitWeaponSlots();
	Character = Cast<ABaseCharacter>(GetOwner());
}

//CAI NAY LA CHO ITEM

void UInventorySystem::CreateMaxSlot(int32 intMaxSlot)
{
	ItemSlot.Empty();
	for (int32 i = 0; i < intMaxSlot; i++)
	{
		FItemSlot Slot;
		Slot.IsEmpty = true;
		ItemSlot.Add(Slot);
	}
	UE_LOG(LogTemp, Log, TEXT("✅ Đã tạo %d slot trống cho inventory."), intMaxSlot);
	MaxSlot = intMaxSlot;
}


void UInventorySystem::AddItemToSlotFromWorld(AAWorldItem* WorldItem)
{
	if (!WorldItem || !WorldItem->ItemRowName.DataTable) return;

	const FInventoryItemData* ItemData = WorldItem->ItemRowName.GetRow<FInventoryItemData>(TEXT("AddItemFromWorld"));
	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ Không tìm thấy dòng dữ liệu trong DataTable."));
		return;
	}

	// Tìm slot trống
	for (int32 i = 0; i < ItemSlot.Num(); i++)
	{
		if (ItemSlot[i].IsEmpty)
		{
			
			ItemSlot[i].ItemRowName = ItemData->ItemRowName;
			ItemSlot[i].ItemType = ItemData->ItemType;
			ItemSlot[i].SlotQuantity = WorldItem->ItemQuantity;
			ItemSlot[i].AmmoType = ItemData->AmmoType;
			ItemSlot[i].IsEmpty = false;
			UE_LOG(LogTemp, Log, TEXT("🎒 Đã thêm item %s vào slot %d | Type: %s | AmmoType: %s"),
				*ItemData->ItemRowName.ToString(),
				i,
				*UEnum::GetValueAsString(ItemData->ItemType),
				*UEnum::GetValueAsString(ItemData->AmmoType));

			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("⚠️ Không còn slot trống để thêm item!"));
}



void UInventorySystem::LogAllItemSlots()
{
	for (int32 i = 0; i < ItemSlot.Num(); i++)
	{
		const FItemSlot& Slot = ItemSlot[i];

		if (Slot.IsEmpty)
		{
			UE_LOG(LogTemp, Log, TEXT("🧺 Slot %d: [TRỐNG]"), i);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("📦 Slot %d: %s | Type: %s | AmmoType: %s | Quantity: %d"),
				i,
				*Slot.ItemRowName.ToString(),
				*UEnum::GetValueAsString(Slot.ItemType),
				*UEnum::GetValueAsString(Slot.AmmoType),
				Slot.SlotQuantity
			);
		
		}
	}
}
































//Nhung cai nay dung cho sung xong roi (KHONG DUOC XOA)*******
EWeaponSlot UInventorySystem::GetSlotFromWeaponType(E_Weapon_Type WeaponType)
{
	switch (WeaponType)
	{
	case E_Weapon_Type::Rifle:
		return EWeaponSlot::Main;
	case E_Weapon_Type::Pistol:
		return EWeaponSlot::Secondary;
	case E_Weapon_Type::Melee:
		return EWeaponSlot::Melee;
	default:
		return EWeaponSlot::Unarmed;
	}
}



void UInventorySystem::AddWeaponToSlot(ABaseWeapon* HoveredWeapon, EWeaponSlot Slot)
{
	if (!HoveredWeapon) return;
	FWeaponInventoryV2 NewSlotData;


	NewSlotData.WeaponClass = HoveredWeapon->GetClass();
	NewSlotData.RowName = HoveredWeapon->WeaponRowName.RowName;
	NewSlotData.WeaponType = HoveredWeapon->WeaponType;
	NewSlotData.WeaponInventoryAmmo = HoveredWeapon->CurrentAmmo;
	NewSlotData.FireMode = HoveredWeapon->FireMode;
	NewSlotData.bIsOccupied = true;
	NewSlotData.WeaponInstance = HoveredWeapon;
	WeaponSlotMap.Add(Slot, NewSlotData);

	UE_LOG(LogTemp, Log, TEXT("✅ AddWeaponToSlot: [%s] ➤ %s | Ammo: %d | FireMode: %s | Row: %s"),
		*UEnum::GetValueAsString(Slot),
		*HoveredWeapon->GetName(),
		HoveredWeapon->CurrentAmmo,
		*UEnum::GetValueAsString(HoveredWeapon->FireMode),
		*HoveredWeapon->WeaponRowName.RowName.ToString());

}

ABaseWeapon* UInventorySystem::GetWeaponFromSlot(EWeaponSlot Slot)
{
	if(!WeaponSlotMap.Contains(Slot)) return nullptr;

	FWeaponInventoryV2& SlotData = WeaponSlotMap[Slot];
	if (!SlotData.bIsOccupied || !SlotData.WeaponClass) return nullptr;

	if (SlotData.WeaponInstance && IsValid(SlotData.WeaponInstance))
	{
		return SlotData.WeaponInstance;
	}
	return nullptr;
}

void UInventorySystem::UpdateWeaponAmmoInSlot(EWeaponSlot Slot, int32 NewAmmo)
{
	if (!WeaponSlotMap.Contains(Slot)) return;

	FWeaponInventoryV2& SlotData = WeaponSlotMap[Slot];
	if (SlotData.bIsOccupied)
	{
		SlotData.WeaponInventoryAmmo = NewAmmo;

		UE_LOG(LogTemp, Log, TEXT("💾 Ammo đã được cập nhật: Slot %s -> Ammo: %d"),
			*UEnum::GetValueAsString(Slot),
			NewAmmo
		);
	}
}



void UInventorySystem::SetWeaponDataFromSlotWhenDrop(ABaseWeapon* CurrentWeapon, EWeaponSlot Slot)
{
	if (!CurrentWeapon || !WeaponSlotMap.Contains(Slot)) return;

	const FWeaponInventoryV2& SlotData = WeaponSlotMap[Slot];
	CurrentWeapon->AmmoType = SlotData.AmmoType;
	CurrentWeapon->CurrentAmmo = SlotData.WeaponInventoryAmmo;
	CurrentWeapon->WeaponType = SlotData.WeaponType;
	CurrentWeapon->WeaponRowName.RowName = SlotData.RowName;
	CurrentWeapon->FireMode = SlotData.FireMode;
	CurrentWeapon->WeaponRowName.DataTable = BaseCharacterData->WeaponDatabase;

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,
		FString::Printf(TEXT("SetAmmoFromSlot -> Ammo: %d | AmmoType: %s | Row: %s"),
			CurrentWeapon->CurrentAmmo,
			*UEnum::GetValueAsString(CurrentWeapon->AmmoType),
			*SlotData.RowName.ToString()));
}

int32 UInventorySystem::TakeAmmoFromSlot(EAmmoType AmmoType, int32 AmmoToTake)
{
	int32 TakeAmmo = 0;


	for (FItemSlot& Slot : ItemSlot)
	{
		UE_LOG(LogTemp, Warning, TEXT("Slot check → Type: %s | Qty: %d | Empty: %d"),
			*UEnum::GetValueAsString(Slot.AmmoType), Slot.SlotQuantity, Slot.IsEmpty);

		if (Slot.IsEmpty || Slot.AmmoType != AmmoType || Slot.SlotQuantity <= 0)
			continue;

		int32 CanTake = FMath::Min(Slot.SlotQuantity, AmmoToTake - TakeAmmo);
		Slot.SlotQuantity -= CanTake;
		TakeAmmo += CanTake;

		if (Slot.SlotQuantity <= 0)
			Slot.IsEmpty = true;

		if (TakeAmmo >= AmmoToTake)
			break;
	}

	UE_LOG(LogTemp, Warning, TEXT("AmmoTaken: %d / %d"), TakeAmmo, AmmoToTake);
	return TakeAmmo;
}


void UInventorySystem::InitWeaponSlots()
{
	WeaponSlotMap.Add(EWeaponSlot::Main, FWeaponInventoryV2());
	WeaponSlotMap.Add(EWeaponSlot::Secondary, FWeaponInventoryV2());
	WeaponSlotMap.Add(EWeaponSlot::Melee, FWeaponInventoryV2());
	WeaponSlotMap.Add(EWeaponSlot::Unarmed, FWeaponInventoryV2());
}



