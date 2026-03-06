#pragma once

#include "CoreMinimal.h"
#include "EWeaponState.generated.h"
class ABaseWeapon;

UENUM(BlueprintType)
enum class E_Weapon_Type : uint8
{
    Unarmed UMETA(DisplayName = "Unarmed"),
    Melee   UMETA(DisplayName = "Melee"),
    Pistol  UMETA(DisplayName = "Pistol"),
    Rifle   UMETA(DisplayName = "Rifle")
};
UENUM(BlueprintType)
enum class E_WeaponMode : uint8
{
	Semi		UMETA(DisplayName = "Semi"),
	FullAuto	UMETA(DisplayName = "FullAuto"),
	Burst		UMETA(DisplayName = "Burst")
};
UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Main		UMETA(DisplayName = "Main"),
	Secondary	UMETA(DisplayName = "Secondary"),
	Melee		UMETA(DisplayName = "Melee"),
	Unarmed		UMETA(DisplayName = "Unarmed")
};
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Ammo		UMETA(DisplayName = "ItemAmmo"),
	Health		UMETA(DisplayName = "ItemHealth"),
	Shield		UMETA(DisplayName = "ItemShield")
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Melee		UMETA(DisplayName = "Melee"),
	Rifle		UMETA(DisplayName = "RifleAmmo"),
	Pistol		UMETA(DisplayName = "PistolAmmo"),
	Shotgun		UMETA(DisplayName = "ShotGunAmmo")
};
//Cai nay la cho ITem
USTRUCT(BlueprintType)
struct FInventoryItemData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

};


USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FName ItemRowName;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(BlueprintReadWrite)
	EAmmoType AmmoType;

	UPROPERTY(BlueprintReadWrite)
	int32 SlotQuantity;

	UPROPERTY(BlueprintReadWrite)
	bool IsEmpty = true;
};

















//Nhung cai nay dung cho sung xong roi (KHONG DUOC XOA)*******
USTRUCT(BlueprintType)
struct FWeaponDataRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_Weapon_Type WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_WeaponMode FireMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
};


USTRUCT(BlueprintType)
struct FWeaponInventoryV2
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<ABaseWeapon> WeaponClass;

	UPROPERTY(BlueprintReadWrite)
	FName RowName;

	UPROPERTY(BlueprintReadWrite)
	E_Weapon_Type WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_WeaponMode FireMode;

	UPROPERTY(BlueprintReadWrite)
	int32 WeaponInventoryAmmo = 0 ;

	UPROPERTY(BlueprintReadWrite)
	bool bIsOccupied = false;

	UPROPERTY(BlueprintReadWrite)
	ABaseWeapon* WeaponInstance = nullptr;
};