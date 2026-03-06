#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/EWeaponState.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FHitSomethingDelegate, const FHitResult&, HitResult);
class ABaseCharacter;
class ABaseWeapon;
class UInventorySystem;
class UMyUserWidget;
class ICombatInterface; 
class UBaseCharacterData;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAMEANIMATIONSAMPLE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	void SetupCombatComponent(UBaseCharacterData* BCD);
	void SwitchWeapon(EWeaponSlot Slot);
	void PickupWeapon();
	void SwapWeapon(EWeaponSlot Slot);
	void AttachWeaponToComponent();
	void AttachWeaponToBack(E_Weapon_Type WeaponType);
	void CreateWeaponSlot();
	void DropWeapon(ABaseWeapon* Weapon);
	void ReloadWeapon();
	void Fire();
	void DisableWeaponOverlap(ABaseWeapon* Weapon);
	void StartFiring();
	void StopFiring();
	void FullAutoFire();
	void DelayCanFire();
	void BurstFire();

	void LineTrace_CameraToMuzzle(FHitResult& OutHit, float TraceDistance);

	FHitSomethingDelegate HitSomethingDelegate;
	UPROPERTY()
	ABaseCharacter* Character;

	UPROPERTY()
	UMyUserWidget* InventoryWidget;

	UPROPERTY()
	UInventorySystem* Inventory;

	UPROPERTY()
	FDataTableRowHandle WeaponRowName;

	UPROPERTY()
	ABaseWeapon* HoveredWeapon;

	UPROPERTY()
	ABaseWeapon* CurrentWeapon;

	UPROPERTY()
	ABaseWeapon* WeaponInSlot;
	UPROPERTY()
	int32 BurstShotsFired = 0;
	UPROPERTY(EditDefaultsOnly)
	int32 MaxBurstCount = 3;
	FTimerHandle FullAutoHandle;
	FTimerHandle BurstHandle;
	FTimerHandle DelayHandle;
	UPROPERTY()
	bool bCanFire = true;
private:
	bool bHasWeapon;
	UPROPERTY()
	UBaseCharacterData* BaseCharacterData;

	UPROPERTY()
	TScriptInterface<ICombatInterface> CombatInterface;

	void HandleHitResult(const FHitResult& HitResult);
protected:

	virtual void BeginPlay() override;

};