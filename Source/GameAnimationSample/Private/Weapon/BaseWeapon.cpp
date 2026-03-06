// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"
#include "BaseCharacter.h"
// Sets default values
ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaSphere->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &ABaseWeapon::OnSphereOverlapEnd);

	ShellProjectiel = CreateDefaultSubobject<UArrowComponent>(TEXT("ShellProjectlie"));
	ShellProjectiel->SetupAttachment(RootComponent);
	ShellProjectiel->SetUsingAbsoluteLocation(false);
	ShellProjectiel->SetUsingAbsoluteRotation(false);
	BulletProjectiel = CreateDefaultSubobject<UArrowComponent>(TEXT("BulletProjectiel"));
	BulletProjectiel->SetUsingAbsoluteLocation(false);
	BulletProjectiel->SetUsingAbsoluteRotation(false);
	BulletProjectiel->SetupAttachment(RootComponent);
	InitWeaponFireRate();
}




// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = 0;
	const FWeaponDataRow* WeaponData = WeaponRowName.GetRow<FWeaponDataRow>(TEXT("Init"));
	if (WeaponData)
	{
		WeaponType = WeaponData->WeaponType;
		CurrentAmmo = WeaponData->MaxAmmo;
		FireMode = WeaponData->FireMode;
		Damage = WeaponData->Damage;
		AmmoType = WeaponData->AmmoType;
	}
}

void ABaseWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);
	if (Character)
	{
		Character->Combat->HoveredWeapon = this;
		bIsOverlapping = true;

		const FWeaponDataRow* WeaponData = WeaponRowName.GetRow<FWeaponDataRow>(TEXT("Overlap"));
		if (WeaponData)
		{
			// Gửi lên màn hình UI hoặc log
			UE_LOG(LogTemp, Log, TEXT("🔫 Hovering weapon: %s | Fire mode: %s | Damage: %.1f | MaxAmmo: %d"),
				*WeaponRowName.RowName.ToString(),
				*UEnum::GetValueAsString(WeaponData->FireMode),
				WeaponData->Damage,
				WeaponData->MaxAmmo

			);
		}
	}
}

void ABaseWeapon::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);
	if (Character && Character->Combat && Character->Combat->HoveredWeapon == this)
	{
		Character->Combat->HoveredWeapon = nullptr;
		Character->Combat->CurrentWeapon;
		Character->Combat->WeaponInSlot;
		bIsOverlapping = false;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Đã rời khỏi vùng hover vũ khí"));
	}
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsOverlapping)
	{
		const FWeaponDataRow* WeaponData = WeaponRowName.GetRow<FWeaponDataRow>(TEXT("TickLog"));
		if (WeaponData)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 0.f, FColor::Green,
				FString::Printf(TEXT("Đang đứng gần: %s | Ammo: %d"),
					*WeaponRowName.RowName.ToString(),
					WeaponData->MaxAmmo)
			);
		}
	}
}

void ABaseWeapon::FireTrace()
{
	FHitResult HitResult;
	FVector Start = BulletProjectiel->GetComponentLocation();
	FVector End = Start + BulletProjectiel->GetForwardVector() * 10000.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		Params
	);

	if (bHit)
	{
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, 2.0f, 0, 1.0f);
		DrawDebugSphere(GetWorld(), HitResult.Location, 10.f, 12, FColor::Green, false, 2.0f);
		UE_LOG(LogTemp, Warning, TEXT("🎯 Hit: %s"), *HitResult.GetActor()->GetName());
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::White, false, 2.0f, 0, 1.0f);
	}
}

void ABaseWeapon::InitWeaponFireRate()
{
	switch (WeaponType)
	{
	case E_Weapon_Type::Rifle:
		FireRate = 1.0f;
		break;
	case E_Weapon_Type::Pistol:
		FireRate = 1.5f;
		break;
	case E_Weapon_Type::Melee:
		FireRate = 0.8f;
		break;
	default:
		FireRate = 0.3f;
		break;
	}
}

FVector ABaseWeapon::GetSocketLocation(FName Socketlocation)
{
	if (WeaponMesh && WeaponMesh->DoesSocketExist(Socketlocation))
	{
		return WeaponMesh->GetSocketLocation(Socketlocation);
	}

	return FVector::ZeroVector;
}

