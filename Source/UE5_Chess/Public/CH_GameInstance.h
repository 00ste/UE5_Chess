// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CH_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE5_CHESS_API UCH_GameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	// score value for human player
	UPROPERTY(EditAnywhere)
	int32 ScoreHumanPlayer = 0;

	// score value for AI player
	UPROPERTY(EditAnywhere)
	int32 ScoreAiPlayer = 0;

	// message to show every turn
	UPROPERTY(EditAnywhere)
	FString CurrentTurnMessage = "Current Player";

public:
	// increment the score for human player
	void IncrementScoreHumanPlayer();

	// increment the score for AI player
	void IncrementScoreAiPlayer();

	// get the score for human player
	UFUNCTION(BlueprintCallable)
	int32 GetScoreHumanPlayer() const;

	// get the score for AI player
	UFUNCTION(BlueprintCallable)
	int32 GetScoreAiPlayer() const;

	// get the current turn message
	UFUNCTION(BlueprintCallable)
	FString GetTurnMessage() const;

	// set the turn message
	UFUNCTION(BlueprintCallable)
	void SetTurnMessage(FString Message);
};
