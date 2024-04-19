// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "MoveButton.generated.h"


UCLASS()
class UE5_CHESS_API UMoveButton : public UButton
{
	GENERATED_BODY()
public:
	UMoveButton(const FObjectInitializer& ObjectInitializer);

	// The index of the ChessMove from the beginning of the game (0-indexed)
	UPROPERTY()
		uint32 MoveIndex = 69420;

private:
	UFUNCTION()
		void OnButtonClickedHandler();
};
