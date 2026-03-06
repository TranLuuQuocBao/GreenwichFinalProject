// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/EWeaponState.h"
#include "Components/SphereComponent.h" 
#include "Components/ArrowComponent.h"
#include "BaseWeapon.generated.h"

class ABaseCharacter;
UCLASS(Blueprintable)
class GAMEANIMATIONSAMPLE_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	E_Weapon_Type WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EAmmoType AmmoType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 CurrentAmmo = 0 ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	E_WeaponMode FireMode;

	UPROPERTY()
	ABaseWeapon* CurrentWeapon;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable")
	FDataTableRowHandle WeaponRowName;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USphereComponent* AreaSphere;
	bool bIsOverlapping = false;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UArrowComponent* ShellProjectiel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UArrowComponent* BulletProjectiel;

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bCanFire ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireRate ;




public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	void Tick(float DeltaTime);
	void FireTrace();

	void InitWeaponFireRate();

	FVector GetSocketLocation(FName Socketlocation);


};
