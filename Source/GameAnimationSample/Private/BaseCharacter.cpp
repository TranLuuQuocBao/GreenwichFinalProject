// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Data/EWeaponState.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Blueprint/UserWidget.h"
#include "Widget/MyUserWidget.h"
#include "Widget/HealthWidget.h"
#include "Widget/CrosshairWidget.h"
#include "Animation/BaseAnimInstance.h"
#include "DataAsset/EnhanceInputData.h"
#include "DataAsset/BaseCharacterData.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Inventory = CreateDefaultSubobject<UInventorySystem>(TEXT("InventoryComponent"));
	PickupComponent = CreateDefaultSubobject<UItemPickupComponent>(TEXT("PickupComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	AddMappingContextForCharacter();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

			
		if (!EnhanceInputData) return;
			EnhancedInputComponent->BindAction(EnhanceInputData->IA_ShootandAttack, ETriggerEvent::Triggered, this, &ABaseCharacter::Shoot);
			EnhancedInputComponent->BindAction(EnhanceInputData->IA_Look, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
			EnhancedInputComponent->BindAction(EnhanceInputData->IA_Move, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
			EnhancedInputComponent->BindAction(EnhanceInputData->IA_SwitchToMelee, ETriggerEvent::Completed, this, &ABaseCharacter::SwitchToMelee);
			EnhancedInputComponent->BindAction(EnhanceInputData->IA_SwitchToPistol, ETriggerEvent::Completed, this, &ABaseCharacter::SwitchToPistol);
			EnhancedInputComponent->BindAction(EnhanceInputData->IA_SwitchToRifle, ETriggerEvent::Completed, this, &ABaseCharacter::SwitchToRifle);
			EnhancedInputComponent->BindAction(EnhanceInputData->IA_Pickup, ETriggerEvent::Completed, this, &ABaseCharacter::Pickup);
			EnhancedInputComponent->BindAction(EnhanceInputData->IA_OpenInventoy, ETriggerEvent::Started, this, &ABaseCharacter::OpenInventory);
			EnhancedInputComponent->BindAction(EnhanceInputData->IA_Reload, ETriggerEvent::Completed, this, &ABaseCharacter::ReloadWeapon);
			EnhancedInputComponent->BindAction(EnhanceInputData->IA_Aim, ETriggerEvent::Triggered, this, &ABaseCharacter::ShowCrosshair);
	}
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat)
	{
		Combat->HitSomethingDelegate.BindDynamic(this, &ABaseCharacter::HandleHitSomthing);
		Combat->SetupCombatComponent(BaseCharacterData);
	}
	
	if (Inventory)
		Inventory->SetupInventoryComponent(BaseCharacterData);

	if (HealthComponent)
		HealthComponent->SetupHealthComponent(BaseCharacterData);
}

void ABaseCharacter::BeginPlay()
{


	Super::BeginPlay();	
	Combat->Character = this;
	Inventory->Character = this;
	WeaponSlot = EWeaponSlot::Unarmed;
	//Health Widget
	Inventory->CreateMaxSlot(BaseCharacterData->MaxSlot);
	bInventoryOpen = false;
	HealthWidget = CreateWidget<UHealthWidget>(GetWorld(), BaseCharacterData->HealthWidgetClass);
	HealthWidget->HealthComponent = HealthComponent;
	CrosshairWidget = CreateWidget<UCrosshairWidget>(GetWorld(), BaseCharacterData->CrosshairWidgetClass);
	InventoryWidget = CreateWidget<UMyUserWidget>(GetWorld(), BaseCharacterData->InventoryBlueprintWidgetClass);
	OnTakePointDamage.AddDynamic(this, &ABaseCharacter::HandleTakePointDamage);
	SetUpHealthWidget();
}

void ABaseCharacter::SetUpHealthWidget()
{
	HealthComponent->Character = this;
	HealthComponent->SetupMaxHealth();

	if (BaseCharacterData->HealthWidgetClass && HealthComponent)
	{
		
		if (HealthWidget)
		{
			HealthWidget->AddToViewport();
			HealthWidget->UpdatePlayerHealthBar(HealthComponent->CurrentHealth, HealthComponent->MaxHealth);
			HealthWidget->UpdateEnemyHealthBar(HealthComponent->CurrentHealth, HealthComponent->MaxHealth);
			HealthWidget->HideEnemyHealth();
		}
	}
}

void ABaseCharacter::AddMappingContextForCharacter()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
		PlayerController->GetLocalPlayer();
	ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());


		if (Subsystem && EnhanceInputData)
		Subsystem->AddMappingContext(EnhanceInputData->InputMappingContext, 0);
}


void ABaseCharacter::ShowCrosshair(const FInputActionInstance& Instance)
{
	

	bool bPressed = Instance.GetValue().Get<bool>();

	if (bPressed)
	{
		bWantsToAim = true;

		if (CrosshairWidget && !CrosshairWidget->IsInViewport())
		{
			CrosshairWidget->AddToViewport();
		}
	}
	else
	{
		bWantsToAim = false;

		if (CrosshairWidget && CrosshairWidget->IsInViewport())
		{
			CrosshairWidget->RemoveFromParent();
		}
	}
}

void ABaseCharacter::HideCrosshair()
{
	if (CrosshairWidget && CrosshairWidget->IsInViewport())
	{
		CrosshairWidget->RemoveFromParent();
	}
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	
	const FVector2D LookValue = Value.Get<FVector2D>();
	AddControllerYawInput(LookValue.X);
	AddControllerPitchInput(-LookValue.Y);


}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveValue = Value.Get<FVector2D>();
	
	//Di chuyen len xuong
	FRotator MyControllerRotation = FRotator(0.0, GetControlRotation().Yaw, 0.0);
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(MyControllerRotation);

	AddMovementInput(ForwardVector, MoveValue.Y);

	const FVector RightVector = UKismetMathLibrary::GetRightVector(MyControllerRotation);
	AddMovementInput(RightVector, MoveValue.X);


}





void ABaseCharacter::SwitchToRifle()
{

	WeaponSlot = EWeaponSlot::Main;
	Combat->SwitchWeapon(EWeaponSlot::Main);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Switch to Rifle"));
}

void ABaseCharacter::SwitchToPistol()
{
	WeaponSlot = EWeaponSlot::Secondary;
	Combat->SwitchWeapon(EWeaponSlot::Secondary);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Switch to Pistol"));
}


void ABaseCharacter::SwitchToMelee()
{
	WeaponSlot = EWeaponSlot::Melee;
	Combat->SwitchWeapon(EWeaponSlot::Melee);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, TEXT("Switch to Melee"));
}

void ABaseCharacter::Shoot(const FInputActionInstance& Instance)
{
	
	bool bPressed = Instance.GetValue().Get<bool>();
	if (bWantsToAim == false) return;
	if (bPressed)
	{
		Combat->StartFiring();
	}
	else
	{
		Combat->StopFiring(); 
	}

}

void ABaseCharacter::StopShoot()
{
	Combat->StopFiring();
}

void ABaseCharacter::HandleHitSomthing(const FHitResult& HitResult)
{
	auto HitActor = HitResult.GetActor();
	FVector ShotDirection = (HitResult.ImpactPoint - HitResult.TraceStart).GetSafeNormal();
	UGameplayStatics::ApplyPointDamage(
		HitActor,
		Combat->CurrentWeapon->Damage,
		ShotDirection,
		HitResult,
		GetController(),
		this,
		UDamageType::StaticClass()
	);

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Handle Hit Something"));
}

void ABaseCharacter::HandleTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	HealthComponent->UpdateHealthByDamage(Damage);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Take Point Damage"));

	if (HealthComponent->CurrentHealth == 0.0f) {
		HealthWidget->HideEnemyHealth();
		HandleDead();
	}
	
}

void ABaseCharacter::HandleDead()
{
	SetLifeSpan(0.2);

	int32 RandomIndex = FMath::RandRange(0, BaseCharacterData->ItemsToSpawn.Num() - 1);
	TSubclassOf<AAWorldItem> RandomItem = BaseCharacterData->ItemsToSpawn[RandomIndex];
	if (RandomItem)
	{
		FVector SpawnLocation = GetActorLocation();
		GetWorld()->SpawnActor<AAWorldItem>(RandomItem, SpawnLocation, FRotator::ZeroRotator);
	}
}

void ABaseCharacter::I_HandleTagetDestroyed()
{
	HealthWidget->HideEnemyHealth();

}




void ABaseCharacter::I_PickupMontage(UAnimMontage* PickupMontage)
{
	PlayAnimMontage(PickupMontage);
}

void ABaseCharacter::I_EnterCombat(float EnemyHealth, float EnemyMaxhealth)
{
	HealthWidget->ShowEnemyHealth();
	HealthWidget->UpdateEnemyHealthBar(EnemyHealth, EnemyMaxhealth);
}

void ABaseCharacter::I_HitTarget(float HealthTarget, float MaxhealthTarget)
{
	HealthWidget->UpdateEnemyHealthBar(HealthTarget, MaxhealthTarget);
}

void ABaseCharacter::I_RequestAttack()
{
	/*Combat->RequestAttack();*/
}



void ABaseCharacter::Pickup()
{	
	PickupComponent->PickupItem();
	Combat->PickupWeapon();

}

void ABaseCharacter::OpenInventory()
{

	if (bInventoryOpen)
	{
		if (InventoryWidget && InventoryWidget->IsInViewport())
		{
			InventoryWidget->RemoveFromParent();
		}

		APlayerController* PC = GetController<APlayerController>();
		if (PC)
		{
			PC->SetInputMode(FInputModeGameOnly());
			PC->bShowMouseCursor = false;
		}

		bInventoryOpen = false;
		return;
	}


	if (InventoryWidget)
	{
		InventoryWidget->InventoryComponent = Inventory;
		InventoryWidget->ItemDataTable = BaseCharacterData->ItemDataTable;
		InventoryWidget->InventorySlotWidgetClass = BaseCharacterData->SlotWidgetBlueprintClass;

		InventoryWidget->AddToViewport();
		InventoryWidget->CreateItemSlots();

		bInventoryOpen = true;

		APlayerController* PC = GetController<APlayerController>();
		if (PC)
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(BaseCharacterData->InventoryWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
		}
	}
}

void ABaseCharacter::ReloadWeapon()
{
	Combat->ReloadWeapon();
}


FTransform ABaseCharacter::GetHandGunTransform() const
{
	if (!Combat || !Combat->CurrentWeapon)
	{
		return FTransform();
	}

	USkeletalMeshComponent* WeaponMesh = Combat->CurrentWeapon->GetWeaponMesh();
	if (!WeaponMesh) return FTransform();

	FTransform LHIKWorldTransform = WeaponMesh->GetSocketTransform(FName("LHIK"), RTS_World);

	FVector OutLocation;
	FRotator OutRotation;

	GetMesh()->TransformToBoneSpace(FName("hand_r"),
		LHIKWorldTransform.GetLocation(),
		LHIKWorldTransform.GetRotation().Rotator(),
		OutLocation,
		OutRotation);


	return FTransform(OutRotation, OutLocation, FVector(1.f, 1.f, 1.f));

}





