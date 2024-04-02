// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "ChessPiece.h"
#include "PlayersList.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UE5_CHESS_API UPlayersList : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetPlayerNames(FString WhiteName, FString BlackName);
	void AddCapturedPiece(PieceType Type, PieceColor Color);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> WhitePlayerName = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> BlackPlayerName = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> WhiteCaptures = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> BlackCaptures = nullptr;
	
};
