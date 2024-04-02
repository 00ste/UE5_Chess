// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
//#include "MovesHistory.h"
//#include "PlayersList.h"
#include "CH_HUD.generated.h"

/**
 * 
 */
UCLASS()
class UE5_CHESS_API ACH_HUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ACH_HUD();
	
private:
	TSubclassOf<UUserWidget> MainHUD;
};
