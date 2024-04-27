// Fill out your copyright notice in the Description page of Project Settings.


#include "CH_WidgetManager.h"

// Sets default values
ACH_WidgetManager::ACH_WidgetManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACH_WidgetManager::ShowPromotionMenu()
{
	PromotionMenuWidget->AddToViewport(0);
}

void ACH_WidgetManager::HidePromotionMenu()
{
	PromotionMenuWidget->RemoveFromParent();
}

void ACH_WidgetManager::ShowGameOverScreen()
{
	GameOverScreenWidget->AddToViewport(0);
}

void ACH_WidgetManager::HideGameOverScreen()
{
	GameOverScreenWidget->RemoveFromParent();
}

// Called when the game starts or when spawned
void ACH_WidgetManager::BeginPlay()
{
	Super::BeginPlay();

	// Create Widgets
	MovesHistoryWidget = CreateWidget<UMovesHistory>(GetGameInstance(), MovesHistoryClass);
	if (MovesHistoryWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing MovesHistoryWidget"));
		return;
	}
	PromotionMenuWidget = CreateWidget<UPromotionMenu>(GetGameInstance(), PromotionMenuClass);
	if (PromotionMenuWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing PromotionMenuWidget"));
		return;
	}
	GameOverScreenWidget = CreateWidget<UGameOverScreen>(GetGameInstance(), GameOverScreenClass);
	if (GameOverScreenWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing GameOverScreenWidget"));
		return;
	}

	MovesHistoryWidget->AddToViewport(0);

}

// Called every frame
void ACH_WidgetManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

