// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayersList.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"

void UPlayersList::SetPlayerNames(FString WhiteName, FString BlackName)
{
	WhitePlayerName->SetText(FText::FromString(WhiteName));
	BlackPlayerName->SetText(FText::FromString(BlackName));
}

void UPlayersList::AddCapturedPiece(PieceType Type, PieceColor Color)
{
	UImage* CapturedPieceImage = NewObject<UImage>();

	// Load the Texture
	FString TexturePath = TEXT("/Game/Textures/") + GetTextureFileName(Type, Color);
	UTexture2D* CapturedPieceTexture = LoadObject<UTexture2D>(
		nullptr,
		*TexturePath
	);
	if (CapturedPieceTexture == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot load texture"));
		return;
	}

	// Assign the Texture to the Image
	FSlateBrush Brush;
	Brush.SetResourceObject(CapturedPieceTexture);
	CapturedPieceImage->SetBrush(Brush);

	// Add the UImage to the respective player's HorizontalBox
	switch (Color)
	{
	case PieceColor::PBLACK:
		BlackCaptures->AddChild(CapturedPieceImage);
		break;
	case PieceColor::PWHITE:
		WhiteCaptures->AddChild(CapturedPieceImage);
		break;
	}
}

void UPlayersList::Clear()
{
	BlackCaptures->ClearChildren();
	WhiteCaptures->ClearChildren();
}

FString UPlayersList::GetTextureFileName(PieceType Type, PieceColor Color)
{
	switch (Color)
	{
	case PieceColor::PBLACK:
		switch (Type)
		{
		case PieceType::BISHOP:
			return "b_bishop_png_1024px.uasset";
		case PieceType::KING:
			return "b_king_png_1024px.uasset";
		case PieceType::KNIGHT:
			return "b_knight_png_1024px.uasset";
		case PieceType::PAWN:
			return "b_pawn_png_1024px.uasset";
		case PieceType::QUEEN:
			return "b_queen_png_1024px.uasset";
		case PieceType::ROOK:
			return "b_rook_png_1024px.uasset";
		default:
			return "";
	}
	case PieceColor::PWHITE:
		switch (Type)
		{
		case PieceType::BISHOP:
			return "w_bishop_png_1024px.uasset";
		case PieceType::KING:
			return "w_king_png_1024px.uasset";
		case PieceType::KNIGHT:
			return "w_knight_png_1024px.uasset";
		case PieceType::PAWN:
			return "w_pawn_png_1024px.uasset";
		case PieceType::QUEEN:
			return "w_queen_png_1024px.uasset";
		case PieceType::ROOK:
			return "w_rook_png_1024px.uasset";
		default:
			return "";
		}
	default:
		return "";
	}
}
