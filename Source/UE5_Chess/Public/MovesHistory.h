// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Indicator.h"
#include "ChessPiece.h"
#include "MoveEntry.h"
#include "MovesHistory.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHistoryClickedSignature, uint32, MovesFromGameStart);


UCLASS(Abstract)
class UE5_CHESS_API UMovesHistory : public UUserWidget
{
	GENERATED_BODY()

public:
	UMovesHistory(const FObjectInitializer& ObjectInitializer);

	// Adds the MoveText of a ChessMove to the widget.
	// The positioning of the MoveText is based on the given Color
	void AddNewMove(FText MoveText, PieceColor Color);

	// Remove all MoveEntries
	void Clear();

	// Broadcasted when a move was clicked from the MovesHistory
	UPROPERTY(BlueprintAssignable)
		FOnHistoryClickedSignature OnHistoryClickedDelegate;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		TObjectPtr<UVerticalBox> MovesContainer = nullptr;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UMoveEntry> MoveEntryBP;
	
	uint32 NEntries = 0;
	uint32 NMoves = 0;


};
