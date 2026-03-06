// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "Data/EWeaponState.h"
#include "CharacterComponent/CombatComponent.h"
#include "CharacterComponent/InventorySystem.h"
#include "CharacterComponent/ItemPickupComponent.h"
#include "CharacterComponent/HealthComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Interface/CombatInterface.h"
#include "BaseCharacter.generated.h"


class UEnhanceInputData;
class UBaseCharacterData;
class UCrosshairWidget;
class UHealthWidget;
UCLASS()
class GAMEANIMATIONSAMPLE_API ABaseCharacter : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:

	ABaseCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void I_PickupMontage(UAnimMontage* PickupMontage) override;
	virtual void I_EnterCombat(float EnemyHealth, float EnemyMaxhealth) override;
	virtual void I_HitTarget(float HealthTarget, float MaxhealthTarget) override;
	virtual void I_RequestAttack() override;
	
protected:
	virtual void BeginPlay() override;

	void SetUpHealthWidget();
	void AddMappingContextForCharacter();
	UFUNCTION()
	virtual void HandleTakePointDamage(
		AActor* DamagedActor,
		float Damage,
		AController* InstigatedBy,
		FVector HitLocation,
		UPrimitiveComponent* FHitComponent,
		FName BoneName,
		FVector ShotFromDirection,
		const UDamageType* DamageType,
		AActor* DamageCauser);
	virtual void HandleDead();
	virtual void I_HandleTagetDestroyed() override;
private:
	void ShowCrosshair(const FInputActionInstance& Instance);
	void HideCrosshair();
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void SwitchToRifle();
	void SwitchToPistol();
	void SwitchToMelee();
	void Shoot(const FInputActionInstance& Instance);
	void Pickup();
	void OpenInventory();
	void ReloadWeapon();
	void StopShoot();
	UFUNCTION()
	void HandleHitSomthing(const FHitResult& HitResult);


public:
	//virtual void I_PickupMontage(UAnimMontage* PickupMontage) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ESwitchWeapon")
	E_Weapon_Type WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ESwitchWeapon")
	EWeaponSlot WeaponSlot;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	class UInventorySystem* Inventory;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	class UCombatComponent* Combat;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	class UItemPickupComponent* PickupComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	class UBaseAnimInstance* AnimInstance;


	UPROPERTY(EditDefaultsOnly, Category = "InventoryWidget")
	UMyUserWidget* InventoryWidget;

	UPROPERTY()
	UCrosshairWidget* CrosshairWidget;

	UPROPERTY()
	UHealthWidget* HealthWidget;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FTransform GetHandGunTransform() const;

	UPROPERTY()
	bool bInventoryOpen;

	UPROPERTY(BlueprintReadWrite, Category = "Input")
	bool bWantsToAim;


private:
	UPROPERTY(EditDefaultsOnly, Category = "DataAsset")
	UEnhanceInputData* EnhanceInputData;

	UPROPERTY(EditDefaultsOnly, Category = "DataAsset")
	UBaseCharacterData* BaseCharacterData;





};