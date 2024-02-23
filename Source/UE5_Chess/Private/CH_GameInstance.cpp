// Fill out your copyright notice in the Description page of Project Settings.


#include "CH_GameInstance.h"

void UCH_GameInstance::IncrementScoreHumanPlayer() { ScoreHumanPlayer++; }
void UCH_GameInstance::IncrementScoreAiPlayer() { ScoreAiPlayer++; }
void UCH_GameInstance::SetTurnMessage(FString Message) { CurrentTurnMessage = Message; }
int32 UCH_GameInstance::GetScoreHumanPlayer() const { return ScoreHumanPlayer; }
int32 UCH_GameInstance::GetScoreAiPlayer() const { return ScoreAiPlayer; }
FString UCH_GameInstance::GetTurnMessage() const { return CurrentTurnMessage; }




