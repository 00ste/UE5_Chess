// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovesHistory.h"
#include "PromotionMenu.h"
#include "PlayersList.h"
#include "CH_WidgetManager.generated.h"


// DELEGATES SIGNATURES
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewMoveSignature, FText, MoveText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPromotionRequestSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPromotionSelectedSignature, uint32, PromotionCandidate);


UCLASS()
class UE5_CHESS_API ACH_WidgetManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACH_WidgetManager();

	// DELEGATE VARIABLES
	// Broadcasted when a new ChessMove is made
	UPROPERTY(BlueprintAssignable)
	FOnNewMoveSignature OnNewMoveDelegate;
	// Broadcasted when a ChessPiece controlled by a HumanPlayer needs to be promoted
	UPROPERTY(BlueprintAssignable)
	FOnPromotionRequestSignature OnPromotionRequestDelegate;
	// Broadcasted when the user selected a PieceType to promote a ChessPiece from the PromotionMenu
	UPROPERTY(BlueprintAssignable)
	FOnPromotionSelectedSignature OnPromotionSelectedDelegate;

	// WIDGETS' GETTERS
	UMovesHistory* GetMovesHistory() { return MovesHistoryWidget; }
	UPromotionMenu* GetPromotionMenu() { return PromotionMenuWidget; }
	UPlayersList* GetPlayersList() { return PlayersListWidget; }

	void ShowPromotionMenu();
	void HidePromotionMenu();
	void ShowMovesHistory();
	void ShowPlayersList();

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
	TObjectPtr<UMovesHistory> MovesHistoryWidget;

	// Promotion Menu
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPromotionMenu> PromotionMenuClass;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPromotionMenu> PromotionMenuWidget;

	// Players List
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayersList> PlayersListClass;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPlayersList> PlayersListWidget;

};
