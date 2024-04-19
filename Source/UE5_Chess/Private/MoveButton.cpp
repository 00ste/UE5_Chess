// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveButton.h"
#include "CH_GameMode.h"

UMoveButton::UMoveButton(const FObjectInitializer& ObjectInitializer)
{
	OnClicked.AddDynamic(this, &UMoveButton::OnButtonClickedHandler);
}

void UMoveButton::OnButtonClickedHandler()
{
	if (MoveIndex != 69420)
	{
		ACH_GameMode* GameMode = Cast<ACH_GameMode>(GetWorld()->GetAuthGameMode());
		GameMode->WidgetManager->GetMovesHistory()->OnHistoryClickedDelegate.Broadcast(MoveIndex);
	}
}