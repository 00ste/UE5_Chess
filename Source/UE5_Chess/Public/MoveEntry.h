// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "MoveButton.h"
#include "Indicator.h"
#include "MoveEntry.generated.h"


/*
UCLASS(Abstract)
class UE5_CHESS_API UMoveButton : public UButton
{
	GENERATED_BODY()

public:
	UMoveButton(const FObjectInitializer& ObjectInitializer);
	UPROPERTY()
	uint32 TotalMoveIndex;
	UFUNCTION()
	void BroadcastHistoryClickedDelegate();
};
*/


UCLASS(Abstract)
class UE5_CHESS_API UMoveEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	UMoveEntry(const FObjectInitializer& ObjectInitializer);
	void SetWhiteMove(FText MoveText);
	void SetBlackMove(FText MoveText);
	void SetIndex(uint32 EntryIndex);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		TObjectPtr<UMoveButton> MoveWhiteButton = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		TObjectPtr<UMoveButton> MoveBlackButton = nullptr;

private:
	// The index the MoveEntry within the MoveHistory, which
	// contains two moves: IT'S NOT THE INDEX OF ONE MOVE
	UPROPERTY()
		uint32 Index;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		TObjectPtr<UTextBlock> EntryIndexText = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		TObjectPtr<UTextBlock> MoveWhiteText = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		TObjectPtr<UTextBlock> MoveBlackText = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		TObjectPtr<UImage> BackgroundColor = nullptr;
};
