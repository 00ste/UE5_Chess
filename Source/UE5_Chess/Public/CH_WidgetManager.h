// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovesHistory.h"
#include "PromotionMenu.h"
#include "GameOverScreen.h"
#include "CH_WidgetManager.generated.h"


UCLASS()
class UE5_CHESS_API ACH_WidgetManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACH_WidgetManager();

	// WIDGETS' GETTERS
	UMovesHistory* GetMovesHistory() { return MovesHistoryWidget; }
	UPromotionMenu* GetPromotionMenu() { return PromotionMenuWidget; }
	UGameOverScreen* GetGameOverScreen() { return GameOverScreenWidget; }

	void ShowPromotionMenu();
	void HidePromotionMenu();
	void ShowGameOverScreen();
	void HideGameOverScreen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Moves History
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMovesHistory> MovesHistoryClass;
	UPROPERTY(EditDefaultsOnly)
	UMovesHistory* MovesHistoryWidget;

	// Promotion Menu
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPromotionMenu> PromotionMenuClass;
	UPROPERTY(EditDefaultsOnly)
	UPromotionMenu* PromotionMenuWidget;

	// Game Over Screen
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameOverScreen> GameOverScreenClass;
	UPROPERTY(EditDefaultsOnly)
	UGameOverScreen* GameOverScreenWidget;

};
