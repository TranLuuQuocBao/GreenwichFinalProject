// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/BaseWeapon.h"  
#include "AWorldItem.h"
#include "Data/EWeaponState.h"
#include "InventorySystem.generated.h"


class ABaseWeapon;
class ABaseCharacter;
class UBaseCharacterData;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEANIMATIONSAMPLE_API UInventorySystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventorySystem();
	void SetupInventoryComponent(UBaseCharacterData* BCD);
	UPROPERTY()
	UBaseCharacterData* BaseCharacterData;
	UPROPERTY()
	ABaseCharacter* Character;

public:
	//CAI NAY LA CHO ITEM
	UPROPERTY()
	TArray<FItemSlot> ItemSlot;
	UPROPERTY()
	int32 MaxSlot;
	void CreateMaxSlot(int32 intMaxSlot);

	void AddItemToSlotFromWorld(AAWorldItem* WorldItem);

	void LogAllItemSlots();

public:
	//Nhung cai nay dung cho sung xong roi (KHONG DUOC XOA)*******
	UFUNCTION()
	EWeaponSlot GetSlotFromWeaponType(E_Weapon_Type WeaponType);
	UPROPERTY()
	TMap<EWeaponSlot, FWeaponInventoryV2> WeaponSlotMap;
	UFUNCTION()
	void InitWeaponSlots();
	UFUNCTION()
	void AddWeaponToSlot(ABaseWeapon* hoverWeapon, EWeaponSlot slot);
	UFUNCTION()
	ABaseWeapon* GetWeaponFromSlot(EWeaponSlot slot);
	UFUNCTION()
	void UpdateWeaponAmmoInSlot(EWeaponSlot slot, int32 newAmmo);

	void SetWeaponDataFromSlotWhenDrop(ABaseWeapon* CurrentWeapon, EWeaponSlot Slot);


	UFUNCTION()
	int32 TakeAmmoFromSlot(EAmmoType AmmoType, int32 AmmoToTake);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
