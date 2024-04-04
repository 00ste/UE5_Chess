// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
// #include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Indicator.h"
#include "ChessPiece.h"
#include "MoveEntry.h"
#include "MovesHistory.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UE5_CHESS_API UMovesHistory : public UUserWidget
{
	GENERATED_BODY()

public:
	UMovesHistory(const FObjectInitializer& ObjectInitializer);
	void AddNewMove(FText MoveText, PieceColor Color);
protected:
	virtual void NativeConstruct() override;
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UVerticalBox> MovesContainer = nullptr;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMoveEntry> MoveEntryBP;
	UPROPERTY()
	uint32 NEntries = 0;
};
