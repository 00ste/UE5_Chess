// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveEntry.h"
#include "CH_GameMode.h"

UMoveEntry::UMoveEntry(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	SetMinimumDesiredSize(FVector2D(0.0, 25.0));
}

void UMoveEntry::SetWhiteMove(FText MoveText)
{
	MoveWhiteText->SetText(MoveText);
	// The nth entry stores the (2n)th and (2n+1)th moves (0-indexed)
	// the (2n)th move is the white one
	MoveWhiteButton->MoveIndex = Index * 2;
}

void UMoveEntry::SetBlackMove(FText MoveText)
{
	MoveBlackText->SetText(MoveText);
	// The nth entry stores the (2*n)th and (2*n+1)th moves (0-indexed)
	// the (2n+1)th move is the black one
	MoveBlackButton->MoveIndex = Index * 2 + 1;
}

void UMoveEntry::SetIndex(uint32 EntryIndex)
{
	Index = EntryIndex - 1;
	EntryIndexText->SetText(FText::FromString(FString::Printf(TEXT("%d."), EntryIndex)));

	BackgroundColor->SetColorAndOpacity(FLinearColor(
		0.0,
		0.0,
		0.0,
		(EntryIndex % 2) == 0 ? 0.1 : 0.0
	));
}