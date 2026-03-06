// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HealthWidget.h"
#include "CharacterComponent/HealthComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "DataAsset/BaseCharacterData.h"
void UHealthWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


}

void UHealthWidget::UpdatePlayerHealthBar(float Health, float Maxhealth)
{
	if (!HealthComponent || !PlayerHealthBar) return;

	PlayerHealthBar->SetPercent(Health / Maxhealth);
}

void UHealthWidget::UpdateEnemyHealthBar(float Health, float Maxhealth)
{
	if (!HealthComponent || !PlayerEnemyHealthBar) return;

	PlayerEnemyHealthBar->SetPercent(Health / Maxhealth);

	FString HealthString = FString::Printf(TEXT("%d / %d"),
		FMath::CeilToInt(Health), FMath::CeilToInt(Maxhealth));

	if (HealthTextEnemy)
		HealthTextEnemy->SetText(FText::FromString(HealthString));

}

void UHealthWidget::ShowEnemyHealth()
{
	PlayerEnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	HealthTextEnemy->SetVisibility(ESlateVisibility::Visible);
}

void UHealthWidget::HideEnemyHealth()
{
	PlayerEnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	HealthTextEnemy->SetVisibility(ESlateVisibility::Hidden);
}

