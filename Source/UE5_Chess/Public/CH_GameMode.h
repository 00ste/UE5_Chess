// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CH_PlayerInterface.h"
#include "CH_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class UE5_CHESS_API ACH_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

private:
	bool IsGameOver;
	TArray<ICH_PlayerInterface*> Players;

};
