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
	PromotionMenuWidget = CreateWidget<UPromotionMenu>(GetGameInstance(), PromotionMenuClass);
	if (PromotionMenuClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing PromotionMenuClass"));
		return;
	}
	PromotionMenuWidget->AddToViewport(0);
}

void ACH_WidgetManager::HidePromotionMenu()
{
	PromotionMenuWidget->RemoveFromParent();
}

// Called when the game starts or when spawned
void ACH_WidgetManager::BeginPlay()
{
	Super::BeginPlay();

	MovesHistoryWidget = CreateWidget<UMovesHistory>(GetGameInstance(), MovesHistoryClass);
	if (MovesHistoryWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing MovesHistoryWidget"));
		return;
	}
	MovesHistoryWidget->AddToViewport(0);	
}

// Called every frame
void ACH_WidgetManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

