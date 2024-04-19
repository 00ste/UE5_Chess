// Fill out your copyright notice in the Description page of Project Settings.


#include "MovesHistory.h"
#include "CH_GameMode.h"
#include "MoveEntry.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

UMovesHistory::UMovesHistory(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	NEntries = 0;
}

void UMovesHistory::AddNewMove(FText MoveText, PieceColor Color)
{
	NMoves++;
	ACH_GameMode* GameMode = Cast<ACH_GameMode>(GetWorld()->GetAuthGameMode());

	// If the Move is a WHITE Move, a new MoveEntry should be created
	if (Color == PWHITE)
	{
		UUserWidget* EntryWidget = CreateWidget(this, MoveEntryBP);
		MovesContainer->AddChildToVerticalBox(EntryWidget);
		UMoveEntry* Entry = Cast<UMoveEntry>(EntryWidget);
		Entry->SetIndex(++NEntries);
		Entry->SetWhiteMove(MoveText);
		//Entry->MoveWhiteButton->OnClicked.AddDynamic(this, &UMovesHistory::BroadcastHistoryClickedDelegate);
	}
	// If the Move is a BLACK Move, the last MoveEntry should be modified
	else
	{
		UMoveEntry* Entry = Cast<UMoveEntry>(MovesContainer->GetChildAt(NEntries - 1));
		Entry->SetBlackMove(MoveText);
		//Entry->MoveBlackButton->OnClicked.AddDynamic(this, &UMovesHistory::BroadcastHistoryClickedDelegate);
	}
}

void UMovesHistory::Clear()
{
	MovesContainer->ClearChildren();
	NMoves = 0;
	NEntries = 0;
}

void UMovesHistory::NativeConstruct()
{
	Super::NativeConstruct();
}
