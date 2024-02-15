// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CH_PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCH_PlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE5_CHESS_API ICH_PlayerInterface
{
	GENERATED_BODY()

public:
	virtual void OnTurn() = 0;
};
