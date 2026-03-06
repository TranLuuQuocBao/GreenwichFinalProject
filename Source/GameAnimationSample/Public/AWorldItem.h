// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "Data/EWeaponState.h"
#include "AWorldItem.generated.h"

UCLASS()
class GAMEANIMATIONSAMPLE_API AAWorldItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAWorldItem();
	UPROPERTY(VisibleAnywhere, Category = "Item Properties")
	USphereComponent* AreaSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable")
	FDataTableRowHandle ItemRowName;

	bool bIsOverlapping = false;

protected:
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


private:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	UStaticMeshComponent* ItemMesh;


};
