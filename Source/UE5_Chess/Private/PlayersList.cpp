// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayersList.h"
#include "Components/Image.h"

void UPlayersList::SetPlayerNames(FString WhiteName, FString BlackName)
{
	WhitePlayerName->SetText(FText::FromString(WhiteName));
	BlackPlayerName->SetText(FText::FromString(BlackName));
}

void UPlayersList::AddCapturedPiece(PieceType Type, PieceColor Color)
{
	// TODO: IMPLEMENT THIS
	// Create UImage for the captured ChessPiece by loading
	// the corresponding texture files

	// Add the UImage to the respective player's HorizontalBox
}
