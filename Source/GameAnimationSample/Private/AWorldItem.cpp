// Fill out your copyright notice in the Description page of Project Settings.


#include "AWorldItem.h"
#include "BaseCharacter.h"
#include "CharacterComponent/ItemPickupComponent.h"
// Sets default values
AAWorldItem::AAWorldItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);


	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaSphere->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AAWorldItem::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AAWorldItem::OnSphereOverlapEnd);
}

// Called when the game starts or when spawned
void AAWorldItem::BeginPlay()
{
	Super::BeginPlay();
	if (!ItemRowName.DataTable || ItemRowName.RowName.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("ItemRowName chưa gán DataTable hoặc RowName!"));
		return;
	}

	const FInventoryItemData* ItemData = ItemRowName.GetRow<FInventoryItemData>(TEXT("GetItemData"));

	if (!ItemData)
	{
		UE_LOG(LogTemp, Error, TEXT("Không tìm thấy row [%s] trong ItemDataTable"), *ItemRowName.RowName.ToString());
		return;
	}

	ItemQuantity = 0;
	ItemQuantity = ItemData->MaxQuantity;
	AmmoType = ItemData->AmmoType;
	Icon = ItemData->Icon;
}

void AAWorldItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);
	if (Character && Character->PickupComponent)
	{
		Character->PickupComponent->HoveredItem = this;
		bIsOverlapping = true;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Đã vao vùng hover Item"));
	}

}

void AAWorldItem::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);
	if (Character && Character->PickupComponent)
	{
		Character->PickupComponent->HoveredItem = nullptr;
		bIsOverlapping = false;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Đã rời khỏi vùng hover Item"));
	}

}

