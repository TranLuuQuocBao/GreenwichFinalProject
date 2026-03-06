// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

class UHealthComponent;
class UBaseCharacterData;
class UProgressBar;
class UTextBlock;
UCLASS()
class GAMEANIMATIONSAMPLE_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	void UpdatePlayerHealthBar(float Health, float Maxhealth);

	void UpdateEnemyHealthBar(float Health, float Maxhealth);

	void ShowEnemyHealth();
	void HideEnemyHealth();
private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* PlayerHealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* PlayerEnemyHealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthTextEnemy;
public:
	UPROPERTY(BlueprintReadWrite, Category = "Health")
	UHealthComponent* HealthComponent;
	UPROPERTY()
	UBaseCharacterData* BaseCharacterData;
	



};
