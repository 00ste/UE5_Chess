// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveEntry.h"

UMoveEntry::UMoveEntry(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UMoveEntry::SetWhiteMove(FText MoveText)
{
	MoveBlackText->SetText(MoveText);
}

void UMoveEntry::SetBlackMove(FText MoveText)
{
	MoveWhiteText->SetText(MoveText);
}

void UMoveEntry::SetMoveIndex(uint32 Index)
{
	MoveIndex->SetText(FText::FromString(FString::Printf(TEXT("%d."), Index)));

	BackgroundColor->SetColorAndOpacity(FLinearColor(
		0.0,
		0.0,
		0.0,
		(Index % 2) == 1 ? 0.5 : 0.0
	));
}

void UMoveEntry::NativeConstruct()
{
	Super::NativeConstruct();
}
