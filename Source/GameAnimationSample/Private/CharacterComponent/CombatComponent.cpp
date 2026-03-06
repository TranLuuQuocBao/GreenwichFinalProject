// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponent/CombatComponent.h"
#include "Weapon/BaseWeapon.h"     
#include "BaseCharacter.h"
#include "Animation/BaseAnimInstance.h"
#include "Widget/MyUserWidget.h"
#include "Interface/CombatInterface.h"
#include "DataAsset/BaseCharacterData.h"
// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::SetupCombatComponent(UBaseCharacterData* BCD)
{
	BaseCharacterData = BCD;
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ABaseCharacter>(GetOwner());
	CombatInterface = TScriptInterface<ICombatInterface>(GetOwner());
	Inventory = Character->Inventory;
	InventoryWidget = Character->InventoryWidget;
}


//Nhung cai nay dung cho sung xong roi (KHONG DUOC XOA)*******
void UCombatComponent::SwitchWeapon(EWeaponSlot Slot)
{	
	if (!Inventory || !Character) return;


	if(!Inventory->WeaponSlotMap.Contains(Slot) || !Inventory->WeaponSlotMap[Slot].bIsOccupied)
	{
		UE_LOG(LogTemp, Warning, TEXT("❗ Slot không có vũ khí!"));
		return;
	}


	if (CurrentWeapon)
	{
		AttachWeaponToBack(CurrentWeapon->WeaponType);
		CurrentWeapon = nullptr;
	}

	WeaponInSlot = Inventory->GetWeaponFromSlot(Slot);
	if (!WeaponInSlot) return;

	WeaponInSlot->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	DisableWeaponOverlap(WeaponInSlot); 
	DisableWeaponOverlap(CurrentWeapon); 
	AttachWeaponToComponent();
	bHasWeapon = true;

	UE_LOG(LogTemp, Log, TEXT("✅ Đã switch sang weapon: %s"), *CurrentWeapon->GetName());
	
}


void UCombatComponent::PickupWeapon()
{
	if (!HoveredWeapon) return;

	EWeaponSlot SlotHoveredWeapon = Inventory->GetSlotFromWeaponType(HoveredWeapon->WeaponType);
	if (Inventory->WeaponSlotMap.Contains(SlotHoveredWeapon) && Inventory->WeaponSlotMap[SlotHoveredWeapon].bIsOccupied)
	{
		SwapWeapon(SlotHoveredWeapon);
		return;
	}
	//pickup o day
	CreateWeaponSlot();
	if (Character->GetMesh())
	{
		if (CombatInterface )
		{
			CombatInterface->I_PickupMontage(BaseCharacterData->PickupMontage);
			

			FTimerHandle DestroyTimer;
			Character->GetWorldTimerManager().SetTimer(
				DestroyTimer,
				[this]()
				{
					if (HoveredWeapon)
					{
						if (CurrentWeapon) {
							WeaponInSlot = HoveredWeapon;
							AttachWeaponToBack(WeaponInSlot->WeaponType);
							HoveredWeapon = nullptr;
							DisableWeaponOverlap(CurrentWeapon);
						}
						else {
							WeaponInSlot = HoveredWeapon;
							AttachWeaponToBack(WeaponInSlot->WeaponType);
							HoveredWeapon = nullptr;
							DisableWeaponOverlap(CurrentWeapon);
						}
					}
				},
				 0.987f,
				false
			);

			return;
		}

	}
	bHasWeapon = false;
}



void UCombatComponent::SwapWeapon(EWeaponSlot Slot)
{
	if (bHasWeapon == true && CurrentWeapon)
	{
		E_Weapon_Type HoveredType = HoveredWeapon->WeaponType;
		E_Weapon_Type CurrentType = CurrentWeapon->WeaponType;

		if (HoveredType != CurrentType)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("❌ Không thể swap vì khác loại vũ khí"));
			return;
		}

		EWeaponSlot CurrentSlot = Inventory->GetSlotFromWeaponType(CurrentWeapon->WeaponType);
		Inventory->AddWeaponToSlot(CurrentWeapon, CurrentSlot);
		DropWeapon(CurrentWeapon);


		Inventory->AddWeaponToSlot(HoveredWeapon, Slot);
		CurrentWeapon = HoveredWeapon;
		WeaponInSlot = CurrentWeapon;
		AttachWeaponToComponent();

		bHasWeapon = true;
		HoveredWeapon = nullptr;

		// 7. Debug
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
			FString::Printf(TEXT("✅ Đã Swap Weapon: %s"), *CurrentWeapon->GetName()));
	}
}

void UCombatComponent::AttachWeaponToComponent()
{
	//Nay la cua switch
	if (WeaponInSlot){ 
		WeaponInSlot->AttachToComponent(
			Character->GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			FName("WeaponSocket")
		);
		DisableWeaponOverlap(WeaponInSlot);
		CurrentWeapon = WeaponInSlot;
	}

}

void UCombatComponent::DisableWeaponOverlap(ABaseWeapon* Weapon)
{
	if (!Weapon || !Weapon->AreaSphere) return;

	Weapon->AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->AreaSphere->SetGenerateOverlapEvents(false);


}




void UCombatComponent::AttachWeaponToBack(E_Weapon_Type WeaponType)
{
	EWeaponSlot Slot = Inventory->GetSlotFromWeaponType(WeaponType);
	WeaponInSlot = Inventory->GetWeaponFromSlot(Slot);
	FName SocketName = NAME_None;
	switch (WeaponType)
	{
	case E_Weapon_Type::Rifle:
		SocketName = FName("RifleSocket");
		break;
	case E_Weapon_Type::Pistol:
		SocketName = FName("PistolSocket");
		break;
	case E_Weapon_Type::Melee:
		SocketName = FName("MeleeSocket");
		break;
	default:
		break;
	}

	WeaponInSlot->AttachToComponent(
		Character->GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		SocketName
	);
	WeaponInSlot = HoveredWeapon;
	DisableWeaponOverlap(WeaponInSlot);
	HoveredWeapon = nullptr;
}

void UCombatComponent::CreateWeaponSlot()
{
	if (!HoveredWeapon) return;
	EWeaponSlot Slot = Inventory->GetSlotFromWeaponType(HoveredWeapon->WeaponType);
	Inventory->AddWeaponToSlot(HoveredWeapon, Slot);
}

void UCombatComponent::DropWeapon(ABaseWeapon* Weapon)
{
	if (!Weapon) return;
	EWeaponSlot Slot = Inventory->GetSlotFromWeaponType(HoveredWeapon->WeaponType);
	Inventory->SetWeaponDataFromSlotWhenDrop(Weapon, Slot);
	
	// Detach khỏi nhân vật
	Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if (Weapon->AreaSphere)
	{
		Weapon->AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Weapon->AreaSphere->SetGenerateOverlapEvents(true);
	}

	
	// Cập nhật HoveredWeapon
	Weapon = HoveredWeapon;

	UE_LOG(LogTemp, Log, TEXT("Dropped weapon: %s"), *Weapon->GetName());
}

void UCombatComponent::ReloadWeapon()
{

	if (!CurrentWeapon || !Inventory) return;

	const FWeaponDataRow* WeaponData = CurrentWeapon->WeaponRowName.GetRow<FWeaponDataRow>(TEXT("Reload"));
	if (!WeaponData) {

		return;
	}

	EAmmoType AmmoType = WeaponData->AmmoType;
	int32 MaxAmmo = WeaponData->MaxAmmo;
	int32 CurrentAmmo = CurrentWeapon->CurrentAmmo;
	int32 AmmoToReload = MaxAmmo - CurrentAmmo;

	if (AmmoToReload <= 0)
	{
		return;
	}

	int32 AmmoTaken = Inventory->TakeAmmoFromSlot(AmmoType, AmmoToReload);
	CurrentWeapon->CurrentAmmo += AmmoTaken;
	if (Character && Character->bInventoryOpen && Character->InventoryWidget)
	{
		Character->InventoryWidget->CreateItemSlots();
	}
}



void UCombatComponent::Fire() 
{
	if (!CurrentWeapon) return;

	if (CurrentWeapon->CurrentAmmo <= 0) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Hết đạn!"));
		return;
	}


	CurrentWeapon->CurrentAmmo--;
	EWeaponSlot Slot = Inventory->GetSlotFromWeaponType(CurrentWeapon->WeaponType);
	Inventory->UpdateWeaponAmmoInSlot(Slot, CurrentWeapon->CurrentAmmo);
	FHitResult Hit;
	LineTrace_CameraToMuzzle(Hit, 50000.0);
	if (Hit.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("🎯 Trúng: %s tại %s"),
			*Hit.GetActor()->GetName(),
			*Hit.ImpactPoint.ToString());
	}
	UE_LOG(LogTemp, Warning, TEXT("🔥 FIRE CALLED: Ammo = %d"), CurrentWeapon->CurrentAmmo);
	const FWeaponDataRow* WeaponData = CurrentWeapon->WeaponRowName.GetRow<FWeaponDataRow>(TEXT("Fire"));
	if (WeaponData)
	{
		UE_LOG(LogTemp, Log, TEXT("Bắn súng: %s | Damage: %.1f | Ammo còn lại: %d"),
			*CurrentWeapon->WeaponRowName.RowName.ToString(),
			WeaponData->Damage,
			CurrentWeapon->CurrentAmmo
		);
	}
}



void UCombatComponent::StartFiring()
{
	if (!CurrentWeapon) return;

	switch (CurrentWeapon->FireMode)
	{
	case E_WeaponMode::Semi:
		UE_LOG(LogTemp, Warning, TEXT("🔥 FireMode: SEMI"));
		if (!bCanFire) return;
			Fire();
			DelayCanFire();
		break;

	case E_WeaponMode::FullAuto:
		UE_LOG(LogTemp, Warning, TEXT("🔥 FireMode: FULL AUTO"));
		if (CurrentWeapon->WeaponType == E_Weapon_Type::Pistol)
		{
			UE_LOG(LogTemp, Warning, TEXT("🚫 Pistol không hỗ trợ FullAuto"));
			return;
		}
		if (bCanFire)
		{
			Fire();
			DelayCanFire();
		}
		FullAutoFire();
		break;

	case E_WeaponMode::Burst:
		UE_LOG(LogTemp, Warning, TEXT("🔥 FireMode: BURST"));
		BurstFire();
		break;
	default:
		break;
	}
}

void UCombatComponent::StopFiring()
{

		GetWorld()->GetTimerManager().ClearTimer(FullAutoHandle);
		GetWorld()->GetTimerManager().ClearTimer(BurstHandle);
	
}

void UCombatComponent::FullAutoFire()
{
	FTimerDelegate FullAutoDelegate;
	FullAutoDelegate.BindLambda([this]()
		{
			if (!bCanFire) return;
			Fire();
			DelayCanFire();
		});

	GetWorld()->GetTimerManager().SetTimer(
		FullAutoHandle,
		FullAutoDelegate,
		CurrentWeapon->FireRate,
		true
	);
}

void UCombatComponent::DelayCanFire()
{
	bCanFire = false;
	FTimerDelegate DelayDelegate;
	DelayDelegate.BindLambda([this]()
		{
			bCanFire = true;

		});

	GetWorld()->GetTimerManager().SetTimer(
		DelayHandle,
		DelayDelegate,
		CurrentWeapon->FireRate,
		false
	);
}

void UCombatComponent::BurstFire()
{
	FTimerDelegate BurstDelegate;
	BurstDelegate.BindLambda([this]()
		{
			if (!CurrentWeapon || !bCanFire) return;

			if (BurstShotsFired >= MaxBurstCount)
			{
				GetWorld()->GetTimerManager().ClearTimer(BurstHandle);
				bCanFire = true;
				return;
			}

			Fire();
			BurstShotsFired++;
		});

	bCanFire = false;

	GetWorld()->GetTimerManager().SetTimer(
		BurstHandle,
		BurstDelegate,
		CurrentWeapon->FireRate,
		true
	);
}

void UCombatComponent::LineTrace_CameraToMuzzle(FHitResult& OutHit, float TraceDistance)
{


	if (!Character || !Character->Controller || !CurrentWeapon) return;


	FVector CameraLoc;
	FRotator CameraRot;
	Character->Controller->GetPlayerViewPoint(CameraLoc, CameraRot);
	FVector CamEnd = CameraLoc + CameraRot.Vector() * TraceDistance;


	TArray<FHitResult> HitResults;
	FCollisionObjectQueryParams ObjectQuery;
	ObjectQuery.AddObjectTypesToQuery(ECC_Pawn);         
	ObjectQuery.AddObjectTypesToQuery(ECC_WorldStatic);  

	FCollisionQueryParams CameraParams;
	CameraParams.AddIgnoredActor(Character);

	GetWorld()->LineTraceMultiByObjectType(
		HitResults,
		CameraLoc,
		CamEnd,
		ObjectQuery,
		CameraParams
	);

	// 🔹 3. Xác định TargetPoint ưu tiên Pawn
	FVector TargetPoint = CamEnd;
	for (const FHitResult& Hit : HitResults)
	{
		if (Hit.GetActor() && Hit.GetActor()->IsA<APawn>())
		{
			TargetPoint = Hit.ImpactPoint;
			break;
		}
		else if (TargetPoint == CamEnd)
		{
			TargetPoint = Hit.ImpactPoint;
		}
	}


	FVector MuzzleLoc = CurrentWeapon->GetSocketLocation(FName("Muzzle"));
	FVector Direction = (TargetPoint - MuzzleLoc).GetSafeNormal();
	FVector FinalEnd = MuzzleLoc + Direction * TraceDistance;

	FHitResult FinalHit;
	FCollisionQueryParams MuzzleParams;
	MuzzleParams.AddIgnoredActor(Character);

	GetWorld()->LineTraceSingleByChannel(
		FinalHit,
		MuzzleLoc,
		FinalEnd,
		ECC_Visibility,
		MuzzleParams
	);

	// 🔹 Xử lý kết quả
	if (FinalHit.bBlockingHit && FinalHit.GetActor())
	{
		AActor* HitActor = FinalHit.GetActor();

		if (HitActor->IsA<APawn>())
		{

			HandleHitResult(FinalHit);
		}
		else
		{
			return;

		}

		DrawDebugLine(GetWorld(), MuzzleLoc, FinalEnd, FColor::Red, false, 1.f, 0, 1.f);
	}
}



void UCombatComponent::HandleHitResult(const FHitResult& HitResult)
{


	if(HitSomethingDelegate.IsBound())
	HitSomethingDelegate.Execute(HitResult);
}

